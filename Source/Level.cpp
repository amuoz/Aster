#include "Level.h"

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <string>

#include "Config.h"
#include "Sprite.h"

#include "Entity/Block.h"
#include "Entity/Player.h"
#include "Entity/SpikeEnemy.h"
#include "Entity/SwordPowerUp.h"
#include "Entity/SpearPowerUp.h"

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
    InitEnemies();
    InitPowerUps();
    // Create player and add it to shared reference
    CreatePlayer(GetPlayerPosition());
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
    for (auto &actor : Actors)
    {
        if (!actor->IsDestroyed)
        {
            actor->Update(deltaTime, playerAttackHitbox);
        }
    }

    if (Character->IsDestroyed)
    {
        Character->GetActorCollider()->active = false;
        Character.reset();
    }
    else
    {
        Character->Update(deltaTime, playerAttackHitbox);
    }
}

void Level::Draw(SpriteRenderer &renderer, double deltaTime)
{
    auto iterator = Actors.begin();
    while (iterator != Actors.end())
    {
        if ((*iterator)->IsDestroyed)
        {
            iterator = Actors.erase(iterator);
        }
        else
        {
            (*iterator)->Draw(renderer, deltaTime);
            ++iterator;
        }
    }

    if (Character->IsActive())
    {
        Character->Draw(renderer, deltaTime);
    }
}

std::shared_ptr<Player> Level::GetPlayer()
{
    return Character;
}

void Level::InitBlocks(unsigned int levelWidth, unsigned int levelHeight)
{
    int height = static_cast<int>(Tiles.size());
    int width = static_cast<int>(Tiles[0].size()); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / static_cast<float>(height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (Tiles[y][x] == 1) // destroyable
            {
                glm::vec3 pos(unit_width * x, unit_height * y, 0.0f);
                glm::vec3 size(unit_width, unit_height, 0.0f);
                auto blockSprite = std::make_unique<Sprite>("block_solid");
                std::unique_ptr<Actor> blockActor = std::make_unique<Block>(
                    pos, size, std::move(blockSprite), glm::vec3(0.8f, 0.8f, 0.7f));
                blockActor->IsDestroyable = true;
                Actors.push_back(std::move(blockActor));
            }
            else if (Tiles[y][x] > 1) // non-destroyable; now determine its color based on level data
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (Tiles[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (Tiles[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (Tiles[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (Tiles[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec3 pos(unit_width * x, unit_height * y, 0.0f);
                glm::vec3 size(unit_width, unit_height, 0.0f);
                auto blockSprite = std::make_unique<Sprite>("block");
                Actors.push_back(
                    std::make_unique<Block>(pos, size, std::move(blockSprite), color));
            }
        }
    }
}

glm::vec3 Level::GetPlayerPosition()
{
    auto playerPosition = LevelInfo["player"]["position"];
    return glm::vec3(playerPosition[0], playerPosition[1], 0);
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
    const glm::vec3 pos = glm::vec3(position[0], position[1], 0.0f);

    glm::vec3 charScale(1.0f, 1.0f, 1.0f);
    charScale.x = Config::Get()->GetValue(SRC_WIDTH) / ENEMY_SIZE.x;
    charScale.y = Config::Get()->GetValue(SRC_HEIGHT) / ENEMY_SIZE.y;

    auto spikeEnemySprite = std::make_unique<Sprite>("spike_enemy");
    float framePeriod = 0.06f;
    spikeEnemySprite->AddAnimation("spike_enemy_idle", AnimationType::IDLE, framePeriod);

    Actors.push_back(
        std::make_unique<SpikeEnemy>(pos, charScale, std::move(spikeEnemySprite), framePeriod));
}

void Level::InitPowerUps()
{
    auto powerUps = LevelInfo["powerUps"];

    for (auto &powerUp : powerUps)
    {
        std::string powerUpName = powerUp["type"].get<std::string>();
            
        if (powerUpName == "Sword")
        {
            InitSword(powerUp);
        }
        if (powerUpName == "Spear")
        {
            InitSpear(powerUp);
        }
    }
}

void Level::InitSword(nlohmann::json &powerUpInfo)
{
    const glm::vec3 size(50, 50, 0);
    glm::vec3 color = glm::vec3(1, 1, 1);
    auto position = powerUpInfo["position"];
    glm::vec3 pos(position[0], position[1], 0.0f);

    auto blockSprite = std::make_unique<Sprite>("sword_powerup");

    Actors.push_back(
        std::make_unique<SwordPowerUp>(pos, size, std::move(blockSprite), color));
}

void Level::InitSpear(nlohmann::json &powerUpInfo)
{
    const glm::vec3 size(50, 50, 0);
    glm::vec3 color = glm::vec3(1, 1, 1);
    auto position = powerUpInfo["position"];
    glm::vec3 pos(position[0], position[1], 0.0f);

    auto blockSprite = std::make_unique<Sprite>("spear_powerup");

    Actors.push_back(
        std::make_unique<SpearPowerUp>(pos, size, std::move(blockSprite), color));
}


void Level::CreatePlayer(glm::vec3 playerPosition)
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
    playerSprite->AddAnimation("roll_right", AnimationType::DASH_RIGHT, 0.018f);
    playerSprite->AddAnimation("roll_left", AnimationType::DASH_LEFT, 0.018f);

    Character = std::make_shared<Player>(playerPosition, charScale, std::move(playerSprite));
}
