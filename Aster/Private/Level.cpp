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
#include "Block.h"
#include "Sprite.h"
#include "SpikeEnemy.h"
#include "SwordPowerUp.h"

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

    if (Tiles.size() > 0)
    {
        InitBlocks(levelWidth, levelHeight);
        InitEnemies();
        InitPowerUps();
    }
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
}

void Level::Draw(SpriteRenderer &renderer, double deltatime)
{
    for (auto &actor : Actors)
    {
        if (actor->IsDestroyed)
        {
            RemoveFromLevel(actor);
        }
        else
        {
            actor->Draw(renderer, deltatime);
        }
    }
}

void Level::Reset()
{
    for (auto &actor : Actors)
    {
        actor->Reset();
    }
}

void Level::InitBlocks(unsigned int levelWidth, unsigned int levelHeight)
{
    int height = static_cast<int>(Tiles.size());
    int width = static_cast<int>(Tiles[0].size()); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / static_cast<float>(height);

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (Tiles[y][x] == 1) // destroyable
            {
                glm::vec3 pos(unit_width * x, unit_height * y, 0.0f);
                glm::vec3 size(unit_width, unit_height, 0.0f);
                Sprite *blockSprite = new Sprite("block_solid");
                std::unique_ptr<Actor> blockActor = std::make_unique<Block>(
                    pos, size, blockSprite, glm::vec3(0.8f, 0.8f, 0.7f));
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
                Sprite *blockSprite = new Sprite("block");
                std::unique_ptr<Actor> blockActor = std::make_unique<Block>(
                    pos, size, blockSprite, color);
                Actors.push_back(std::move(blockActor));
            }
        }
    }
}

glm::vec3 Level::GetPlayerPosition()
{
    auto playerPosition = LevelInfo["player"]["position"];
    return glm::vec3(playerPosition[0], playerPosition[1], 0);
}

// void Level::AddPlayer(std::shared_ptr<Player> player)
// {
//     Actors.push_back((std::shared_ptr<Actor>)player);
// }

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

    Sprite *spikeEnemySprite = new Sprite("spike_enemy");
    float framePeriod = 0.06f;
    spikeEnemySprite->AddAnimation("spike_enemy_idle", AnimationType::IDLE, framePeriod);

    std::unique_ptr<Actor> enemy = std::make_unique<SpikeEnemy>(
        pos, charScale, spikeEnemySprite, framePeriod);

    Actors.push_back(std::move(enemy));
}

void Level::InitPowerUps()
{
    auto powerUps = LevelInfo["powerUps"];

    for (auto &powerUp : powerUps)
    {
        std::string powerUpType = powerUp["type"].get<std::string>();
        if (powerUpType == "Sword")
        {
            InitSword(powerUp);
        }
    }
}

void Level::InitSword(nlohmann::json &powerUpInfo)
{
    const glm::vec3 POWER_UP_SIZE(22.0f, 15.0f, 0.0f);
    glm::vec3 size(1.0f, 1.0f, 1.0f);
    size.x = Config::Get()->GetValue(SRC_WIDTH) / POWER_UP_SIZE.x;
    size.y = Config::Get()->GetValue(SRC_HEIGHT) / POWER_UP_SIZE.y;

    glm::vec3 color = glm::vec3(1, 1, 1);

    auto position = powerUpInfo["position"];
    glm::vec3 pos(position[0], position[1], 0.0f);

    Sprite *blockSprite = new Sprite("sword_powerup");
    std::unique_ptr<Actor> swordPowerUp = std::make_unique<SwordPowerUp>(
        pos, size, blockSprite, color);

    Actors.push_back(std::move(swordPowerUp));
}

void Level::RemoveFromLevel(std::unique_ptr<Actor> &actor)
{
    auto i = Actors.begin();
    auto end = Actors.end();
    while (i != end)
    {
        if (actor == *i)
        {
            i = Actors.erase(i);
        }
        else
        {
            ++i;
        }
    }
}
