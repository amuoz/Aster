#include "Level.h"

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <string>
#include <algorithm>

#include "Common.h"
#include "Config.h"
#include "Sprite.h"
#include "AsterTypes.h"

#include "Entity/Block.h"
#include "Entity/Building.h"
#include "Entity/Door.h"
#include "Entity/Player.h"
#include "Entity/SpikeEnemy.h"
#include "Entity/PowerUp.h"

Level::Level()
{
}

Level::~Level()
{
}

void Level::Load(std::string file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    Actors.clear();

    std::ifstream in(file);
    LevelInfo = nlohmann::json::parse(in);

    LoadTiles();
    InitBlocks(levelWidth, levelHeight);
    // Create player and add it to shared reference
    CreatePlayer(GetPlayerPosition());
    InitPowerUps();
    InitEnemies();
}

void Level::LoadTiles()
{
    auto &tiles = LevelInfo["tiles"];
    unsigned int i = 0;
    for (std::string line : tiles)
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
        Tiles.push_back(lineNumbers);
        i = i + 1;
    }
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
    auto sortByPosition = [](std::shared_ptr<Actor> a, std::shared_ptr<Actor> b)
    {
        return a->GetPosition().y < b->GetPosition().y;
    };
    // Sort sprites by z-index
    Actors.sort(sortByPosition);

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

void Level::InitBlocks(unsigned int levelWidth, unsigned int levelHeight)
{
    int height = static_cast<int>(Tiles.size());
    int width = static_cast<int>(Tiles[0].size());
    std::vector<std::vector<ActorType> > actorTypes(height, std::vector<ActorType>(width, ActorType::NONE));
    std::vector<std::tuple<int, int> > doorCoordinates;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (Tiles[y][x] == 1)
            {
                actorTypes[y][x] = ActorType::DESTROYABLE_BLOCK;
            }
            else if (Tiles[y][x] == 2)
            {
                actorTypes[y][x] = ActorType::BLOCK;
            }
            else if (Tiles[y][x] == 3)
            {
                actorTypes[y][x] = ActorType::DOOR;
                doorCoordinates.push_back(std::make_tuple(x, y));
            }
        }
    }

    for (auto coords : doorCoordinates)
    {
        std::shared_ptr<Actor> building = CreateBuilding(coords, actorTypes, levelWidth, levelHeight);
        Actors.push_back(building);
    }

    CreateActors(actorTypes, Actors, levelWidth, levelHeight);
}

std::shared_ptr<Actor> Level::CreateBuilding(std::tuple<int, int> coords,
                                             std::vector<std::vector<ActorType> > &actorTypes,
                                             unsigned int levelWidth,
                                             unsigned int levelHeight)
{
    int height = static_cast<int>(Tiles.size());
    int width = static_cast<int>(Tiles[0].size());
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / static_cast<float>(height);

    bool isBuildingProcessed = false;
    std::vector<std::vector<ActorType> > buildingActorsTypes(height, std::vector<ActorType>(width, ActorType::NONE));
    std::vector<std::tuple<int, int, bool> > knownPositions;
    knownPositions.push_back(std::make_tuple(get<0>(coords), get<1>(coords), true));

    std::tuple<int, int> currentCoords = std::make_tuple(
        get<0>(coords),
        get<1>(coords));

    while (!isBuildingProcessed)
    {
        int x = get<0>(currentCoords);
        int y = get<1>(currentCoords);
        buildingActorsTypes[y][x] = actorTypes[y][x];

        DebugBuildingPath(buildingActorsTypes, x, y);

        actorTypes[y][x] = ActorType::NONE;

        std::tuple<int, int, bool> nextTileCoords = GetNextTileCoords(x, y, actorTypes, knownPositions, 0);
        
        get<0>(currentCoords) = get<0>(nextTileCoords);
        get<1>(currentCoords) = get<1>(nextTileCoords);

        if (get<0>(currentCoords) == -1)
        {
            isBuildingProcessed = true;
        }
    }

    std::list<std::shared_ptr<Actor> > buildingActors;
    CreateActors(buildingActorsTypes, buildingActors, levelWidth, levelHeight);

    glm::vec2 doorPosition(unit_width * get<0>(coords), unit_height * get<1>(coords));
    glm::vec3 doorSize(unit_width, unit_height, 0.0f);
    auto doorSprite = std::make_unique<Sprite>("door");
    auto floorSprite = std::make_unique<Sprite>("block");
    glm::vec4 floorColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    std::shared_ptr<Actor> building = std::make_shared<Building>(
        doorPosition, doorSize, std::move(doorSprite), std::move(floorSprite), floorColor, buildingActors);
    return building;
}

std::tuple<int, int, bool> Level::GetNextTileCoords(int x,
                                                    int y,
                                                    std::vector<std::vector<ActorType> > grid,
                                                    std::vector<std::tuple<int, int, bool> > &knownPositions,
                                                    int directionIndex)
{
    auto nextCoords = std::make_tuple(x + DIRECTIONS[directionIndex][0],
                                      y + DIRECTIONS[directionIndex][1],
                                      true);
    auto nextX = get<0>(nextCoords);
    auto nextY = get<1>(nextCoords);
    ActorType tileActor = grid[nextY][nextX];

    // Debug next tile
    // std::cout << "(" << nextX << ", " << nextY << ") ";

    auto areNextCoordsKnown = [&nextCoords](const auto position)
    {
        return get<0>(position) == get<0>(nextCoords) &&
               get<1>(position) == get<1>(nextCoords);
    };

    auto foundPosition = std::find_if(knownPositions.begin(), knownPositions.end(), areNextCoordsKnown);
    if (foundPosition == knownPositions.end())
    {
        if (tileActor == ActorType::BLOCK || tileActor == ActorType::DOOR)
        {
            knownPositions.push_back(nextCoords);
            return nextCoords;
        }
        else
        {
            get<2>(nextCoords) = false;
            knownPositions.push_back(nextCoords);
        }
    }

    if (foundPosition == knownPositions.begin() && directionIndex == 3)
    {
        return std::make_tuple(-1, -1, false);
    }
    else if (directionIndex < 3)
    {
        return GetNextTileCoords(x, y, grid, knownPositions, ++directionIndex);
    }
    else
    {
        auto directions = DIRECTIONS;
        auto isLastBlock = [&foundPosition, directionIndex, directions](const auto position)
        {
            return get<0>(position) == get<0>(*foundPosition) - directions[directionIndex][0] &&
                   get<1>(position) == get<1>(*foundPosition) - directions[directionIndex][1];
        };
        auto lastBlockPosition = std::find_if(knownPositions.begin(), knownPositions.end(), isLastBlock);
        get<2>(*lastBlockPosition) = false;
        int tileIndex = foundPosition - knownPositions.begin();
        if (tileIndex > 0)
        {
            std::tuple<int, int, bool> previousPosition;
            do
            {
                --tileIndex;
                previousPosition = knownPositions[tileIndex];
            } while (get<2>(previousPosition) == false && tileIndex > 0);

            return GetNextTileCoords(get<0>(previousPosition), get<1>(previousPosition), grid, knownPositions, 0);
        }
        else
        {
            return std::make_tuple(-1, -1, false);
        }
    }
}

void Level::CreateActors(std::vector<std::vector<ActorType> > actorTypes,
                         std::list<std::shared_ptr<Actor> > &actors,
                         unsigned int levelWidth,
                         unsigned int levelHeight)
{
    int height = static_cast<int>(Tiles.size());
    int width = static_cast<int>(Tiles[0].size());
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / static_cast<float>(height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (actorTypes[y][x] == ActorType::DESTROYABLE_BLOCK)
            {
                std::shared_ptr<Actor> blockActor = CreateDestroyableBlock(unit_width, unit_height, x, y);
                actors.push_back(blockActor);
            }
            else if (actorTypes[y][x] == ActorType::BLOCK)
            {
                std::shared_ptr<Actor> blockActor = CreateBlock(unit_width, unit_height, x, y);
                actors.push_back(blockActor);
            }
            else if (actorTypes[y][x] == ActorType::DOOR)
            {
                std::shared_ptr<Actor> doorActor = CreateDoor(unit_width, unit_height, x, y);
                actors.push_back(doorActor);
            }
        }
    }
}

std::shared_ptr<Actor> Level::CreateDestroyableBlock(float unit_width, float unit_height, int x, int y)
{
    glm::vec2 pos(unit_width * x, unit_height * y);
    glm::vec3 size(unit_width, unit_height, 0.0f);
    auto blockSprite = std::make_unique<Sprite>("block");
    glm::vec4 color = glm::vec4(0.9f, 0.9f, 1.0f, 1.0f);
    std::shared_ptr<Actor> blockActor = std::make_shared<Block>(
        pos, size, std::move(blockSprite), color);
    blockActor->IsDestroyable = true;

    return blockActor;
}

std::shared_ptr<Actor> Level::CreateBlock(float unit_width, float unit_height, int x, int y)
{
    glm::vec2 pos(unit_width * x, unit_height * y);
    glm::vec3 size(unit_width, unit_height, 0.0f);
    BlockLocation location = GetBlockLocation(x, y);
    auto blockSprite = GetBlockSprite(location);
    glm::vec4 color = glm::vec4(1.0f);
    std::shared_ptr<Actor> blockActor = std::make_shared<Block>(
        pos, size, std::move(blockSprite), color, location);

    return blockActor;
}

std::shared_ptr<Actor> Level::CreateDoor(float unit_width, float unit_height, int x, int y)
{
    glm::vec2 pos(unit_width * x, unit_height * y);
    glm::vec3 size(unit_width, unit_height, 0.0f);
    auto blockSprite = std::make_unique<Sprite>("door");
    glm::vec4 color = glm::vec4(1.0f);
    std::shared_ptr<Actor> doorActor = std::make_shared<Door>(
        pos, size, std::move(blockSprite), color);

    return doorActor;
}

BlockLocation Level::GetBlockLocation(int x, int y)
{
    int top, bottom, left, right;

    if (y == 0)
    {
        TopBlocks(top, bottom, left, right, x, y);
    }
    else if (y == Tiles.size() - 1)
    {
        BottomBlocks(top, bottom, left, right, x, y);
    }
    else
    {
        MiddleBlocks(top, bottom, left, right, x, y);
    }

    return GetBlockLocationByNeighbors(top, bottom, left, right);
}

void Level::TopBlocks(int &top, int &bottom, int &left, int &right, int x, int y)
{
    if (x == 0)
    // left blocks
    {
        left = 2;
        right = Tiles[0][1];
    }
    else if (x == Tiles[0].size() - 1)
    // right blocks
    {
        left = Tiles[0][x - 1];
        right = 2;
    }
    else
    // middle blocks
    {
        left = Tiles[0][x - 1];
        right = Tiles[0][x + 1];
    }

    top = 2;
    bottom = Tiles[y + 1][x];
}

void Level::BottomBlocks(int &top, int &bottom, int &left, int &right, int x, int y)
{
    if (x == 0)
    // left blocks
    {
        left = 2;
        right = Tiles[x][y + 1];
    }
    else if (x == Tiles[y].size() - 1)
    // right blocks
    {
        left = Tiles[y][x - 1];
        right = 2;
    }
    else
    // middle blocks
    {
        left = Tiles[y][x - 1];
        right = Tiles[y][x + 1];
    }

    top = Tiles[y - 1][x];
    bottom = 2;
}

void Level::MiddleBlocks(int &top, int &bottom, int &left, int &right, int x, int y)
{
    if (x == 0)
    // left blocks
    {
        left = 2;
        right = Tiles[y][x + 1];
    }
    else if (x == Tiles[y].size() - 1)
    // right blocks
    {
        left = Tiles[y][x - 1];
        right = 2;
    }
    else
    // middle blocks
    {
        left = Tiles[y][x - 1];
        right = Tiles[y][x + 1];
    }

    top = Tiles[y - 1][x];
    bottom = Tiles[y + 1][x];
}

BlockLocation Level::GetBlockLocationByNeighbors(int top, int bottom, int left, int right)
{
    if (bottom < 2)
    {
        if (left < 2)
        {
            return BlockLocation::BOTTOM_LEFT;
        }
        if (right < 2)
        {
            return BlockLocation::BOTTOM_RIGHT;
        }

        return BlockLocation::BOTTOM;
    }

    if (top < 2)
    {
        if (left < 2)
        {
            return BlockLocation::TOP_LEFT;
        }
        if (right < 2)
        {
            return BlockLocation::TOP_RIGHT;
        }

        return BlockLocation::TOP;
    }

    if (left < 2)
    {
        return BlockLocation::LEFT;
    }
    if (right < 2)
    {
        return BlockLocation::RIGHT;
    }

    return BlockLocation::MIDDLE;
}

std::unique_ptr<Sprite> Level::GetBlockSprite(BlockLocation location)
{
    std::string spriteName = BLOCK_SPRITES[location];
    return std::make_unique<Sprite>(spriteName);
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
    playerSprite->AddAnimation("roll_right", AnimationType::DASH_RIGHT, 0.018f);
    playerSprite->AddAnimation("roll_left", AnimationType::DASH_LEFT, 0.018f);

    Character = std::make_shared<Player>(playerPosition, charScale, std::move(playerSprite));
    Actors.push_back(Character);
}

void Level::DebugBuildingPath(std::vector<std::vector<ActorType> > buildingActorsTypes, int x, int y)
{
    int i = 0;
    int j = 0;
    for (auto line : buildingActorsTypes)
    {
        for (auto cell : line)
        {
            std::string symbol;
            if (x == i && y == j)
                symbol = "o";
            else if (cell == ActorType::NONE)
                symbol = ".";
            else
                symbol = "x";
            std::cout << symbol << " ";

            i++;
        }

        std::cout << endl;

        j++;
        i = 0;
    }
    std::cout << "//////////////////////" << endl;
}
