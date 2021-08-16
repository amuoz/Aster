#include "Level.h"

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>

#include "Common.h"
#include "Config.h"
#include "Sprite.h"
#include "AsterTypes.h"
#include "BuildingManager.h"
#include "TileBuilder.h"

#include "Entity/Player.h"
#include "Entity/SpikeEnemy.h"
#include "Entity/PowerUp.h"

Level::Level(std::shared_ptr<BuildingManager> buildingManager, std::shared_ptr<TileBuilder> tileBuilder)
{
    RoomsManager = buildingManager;
    MapBuilder = tileBuilder;
}

Level::~Level()
{
}

void Level::Load(std::string file)
{
    // clear old data
    Actors.clear();

    std::ifstream in(file);
    LevelInfo = nlohmann::json::parse(in);

    LoadMap();
    InitBlocks();
    // Create player and add it to shared reference
    CreatePlayer(GetPlayerPosition());
    InitPowerUps();
    InitEnemies();
}

void Level::LoadMap()
{
    LoadTiles("tiles", Tiles);
    LoadTiles("interiorTiles", InteriorTiles);

    NumOfTilesX = static_cast<int>(Tiles[0].size());
    NumOfTilesY = static_cast<int>(Tiles.size());
    RoomsManager->SetLevelSize(NumOfTilesX, NumOfTilesY);
    MapBuilder->SetLevelSize(NumOfTilesX, NumOfTilesY);
}

void Level::LoadTiles(std::string tilesName, std::vector<std::vector<int> > &tiles)
{
    auto &tileInfo = LevelInfo[tilesName];
    unsigned int i = 0;

    for (std::string line : tileInfo)
    {
        std::vector<int> lineNumbers;

        size_t last = 0;
        size_t next = 0;
        while ((next = line.find(" ", last)) != string::npos)
        {
            lineNumbers.push_back(stoi(line.substr(last, next - last)));
            last = next + 1;
        }
        lineNumbers.push_back(stoi(line.substr(last)));
        tiles.push_back(lineNumbers);
        i = i + 1;
    }

    NumOfTilesX = static_cast<int>(Tiles[0].size());
    NumOfTilesY = static_cast<int>(Tiles.size());
    RoomsManager->SetLevelSize(NumOfTilesX, NumOfTilesY);
    MapBuilder->SetLevelSize(NumOfTilesX, NumOfTilesY);
}

void Level::Update(float deltaTime, glm::vec4 playerAttackHitbox)
{
    auto iterator = Actors.begin();
    while (iterator != Actors.end())
    {
        if ((*iterator)->IsDestroyed)
        {
            // If actor is only pointed by level list it will be deleted
            (*iterator)->Destroy();
            iterator = Actors.erase(iterator);
        }
        else
        {
            (*iterator)->Update(deltaTime, playerAttackHitbox);
            ++iterator;
        }
    }
}

void Level::Draw(SpriteRenderer &renderer, double deltaTime)
{
    auto sortByZIndex = [](std::shared_ptr<Actor> a, std::shared_ptr<Actor> b)
    {
        return a->ZIndex < b->ZIndex;
    };
    // Sort sprites by z-index
    Actors.sort(sortByZIndex);

    for (auto &actor : Actors)
    {
        if (!actor->IsDestroyed)
        {
            actor->Draw(renderer, deltaTime);
        }
    }
}

void Level::BeginPlay()
{
    for (auto &actor : Actors)
    {
        actor->BeginPlay();
    }
}

std::shared_ptr<Player> Level::GetPlayer()
{
    return Character;
}

void Level::InitBlocks()
{
    std::vector<std::vector<ActorType> > actorTypes(NumOfTilesY,
                                                    std::vector<ActorType>(NumOfTilesX, ActorType::NONE));
    std::vector<std::vector<ActorType> > interiorActorTypes(NumOfTilesY,
                                                            std::vector<ActorType>(NumOfTilesX, ActorType::NONE));
    std::vector<std::tuple<int, int> > doorCoordinates;

    InitActorTypes(actorTypes, Tiles, doorCoordinates, true);
    InitActorTypes(interiorActorTypes, InteriorTiles, doorCoordinates, false);

    for (auto coords : doorCoordinates)
    {
        std::shared_ptr<Actor> building = RoomsManager->CreateBuilding(coords,
                                                                       actorTypes, interiorActorTypes,
                                                                       Tiles, InteriorTiles);
        Actors.push_back(building);
    }

    MapBuilder->CreateActors(Actors, actorTypes, Tiles);
}

void Level::InitActorTypes(std::vector<std::vector<ActorType> > &actorTypes,
                           std::vector<std::vector<int> > tiles,
                           std::vector<std::tuple<int, int> > &doorCoordinates,
                           bool shouldStoreDoors)
{
    for (int y = 0; y < NumOfTilesY; ++y)
    {
        for (int x = 0; x < NumOfTilesX; ++x)
        {
            if (tiles[y][x] == 1)
            {
                actorTypes[y][x] = ActorType::DESTROYABLE_BLOCK;
            }
            else if (tiles[y][x] == 2)
            {
                actorTypes[y][x] = ActorType::BLOCK;
            }
            else if (tiles[y][x] == 3)
            {
                actorTypes[y][x] = ActorType::DOOR;
                if (shouldStoreDoors)
                {
                    doorCoordinates.push_back(std::make_tuple(x, y));
                }
            }
            else if (tiles[y][x] == 4)
            {
                actorTypes[y][x] = ActorType::FLOOR;
            }
        }
    }
}

glm::vec2 Level::GetPlayerPosition()
{
    auto playerPosition = LevelInfo["player"]["position"];
    int xPlayer = (int)playerPosition[0] * Config::Get()->GetValue(CELL_WIDTH);
    int yPlayer = (int)playerPosition[1] * Config::Get()->GetValue(CELL_HEIGHT);
    return glm::vec2(xPlayer, yPlayer);
}

void Level::InitEnemies()
{
    auto enemies = LevelInfo["enemies"];

    for (auto &enemy : enemies)
    {
        std::string enemyType = enemy["type"].get<std::string>();
        if (enemyType == "Spike")
        {
            InitSpike(enemy);
        }
    }
}

void Level::InitSpike(nlohmann::json &enemyInfo)
{
    const glm::vec3 ENEMY_SIZE(16.0f, 9.0f, 0.0f);

    auto position = enemyInfo["position"];
    const glm::vec2 pos(position[0], position[1]);

    glm::vec3 charScale(1.0f, 1.0f, 1.0f);
    charScale.x = Config::Get()->GetValue(SRC_WIDTH) / ENEMY_SIZE.x;
    charScale.y = Config::Get()->GetValue(SRC_HEIGHT) / ENEMY_SIZE.y;

    auto spikeEnemySprite = std::make_unique<Sprite>("spike_enemy");
    float framePeriod = 0.06f;
    spikeEnemySprite->AddAnimation("spike_enemy_idle", AnimationType::IDLE, framePeriod);

    std::shared_ptr<SpikeEnemy> spikeEnemyPtr = std::make_shared<SpikeEnemy>(pos, charScale, std::move(spikeEnemySprite), framePeriod);
    Actors.push_back(spikeEnemyPtr);
}

void Level::InitPowerUps()
{
    auto powerUps = LevelInfo["powerUps"];

    for (auto &powerUp : powerUps)
    {
        std::string powerUpName = powerUp["type"].get<std::string>();
        InitPowerUp(powerUpName, powerUp);
    }
}

void Level::InitPowerUp(std::string name, nlohmann::json &info)
{
    const glm::vec3 size(50, 50, 0);
    glm::vec4 color = glm::vec4(1.0f);
    auto position = info["position"];
    glm::vec2 pos(position[0], position[1]);

    auto blockSprite = std::make_unique<Sprite>(name);
    std::shared_ptr<PowerUp> powerUpPtr = std::make_shared<PowerUp>(
        POWER_UPS[name], pos, size, std::move(blockSprite), color);
    Actors.push_back(powerUpPtr);
}

void Level::CreatePlayer(glm::vec2 playerPosition)
{
    const glm::vec3 PLAYER_SIZE(16.0f, 9.0f, 0.0f);
    glm::vec3 charScale(1.0f, 1.0f, 1.0f);
    charScale.x = Config::Get()->GetValue(SRC_WIDTH) / PLAYER_SIZE.x;
    charScale.y = Config::Get()->GetValue(SRC_HEIGHT) / PLAYER_SIZE.y;

    auto playerSprite = std::make_unique<Sprite>("player");

    playerSprite->AddAnimation("player_walk", AnimationType::WALK, 0.03f);
    playerSprite->AddAnimation("player_idle", AnimationType::IDLE, 0.06f);
    playerSprite->AddAnimation("spear_up", AnimationType::SPEAR_UP, 0.03f);
    playerSprite->AddAnimation("spear_right", AnimationType::SPEAR_RIGHT, 0.03f);
    playerSprite->AddAnimation("spear_down", AnimationType::SPEAR_DOWN, 0.03f);
    playerSprite->AddAnimation("spear_left", AnimationType::SPEAR_LEFT, 0.03f);
    playerSprite->AddAnimation("sword_up", AnimationType::SWORD_UP, 0.03f);
    playerSprite->AddAnimation("sword_right", AnimationType::SWORD_RIGHT, 0.03f);
    playerSprite->AddAnimation("sword_down", AnimationType::SWORD_DOWN, 0.03f);
    playerSprite->AddAnimation("sword_left", AnimationType::SWORD_LEFT, 0.03f);
    playerSprite->AddAnimation("hammer_right", AnimationType::HAMMER_RIGHT, 0.06f);
    playerSprite->AddAnimation("hammer_left", AnimationType::HAMMER_LEFT, 0.06f);
    playerSprite->AddAnimation("roll_up", AnimationType::DASH_UP, 0.018f);
    playerSprite->AddAnimation("roll_right", AnimationType::DASH_RIGHT, 0.018f);
    playerSprite->AddAnimation("roll_down", AnimationType::DASH_DOWN, 0.018f);
    playerSprite->AddAnimation("roll_left", AnimationType::DASH_LEFT, 0.018f);

    Character = std::make_shared<Player>(playerPosition, charScale, std::move(playerSprite));
    Actors.push_back(Character);
}
