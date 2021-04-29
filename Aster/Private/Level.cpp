#include "Level.h"

#include <glm/glm.hpp>
#include <vector>
#include <list>
#include <utility>

#include "Config.h"
#include "Block.h"
#include "ResourceManager.h"
#include "Files.h"
#include "Sprite.h"
#include "SpikeEnemy.h"

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

    std::vector<std::vector<int> > tileData;
    ReadFileLines(file, tileData);

    if (tileData.size() > 0)
    {
        Init(tileData, levelWidth, levelHeight);
    }
}

void Level::Update(float deltaTime, glm::vec4 playerAttackHitbox)
{
    for (auto& actor : Actors)
    {
        if (!actor->IsDestroyed)
        {
            actor->Update(deltaTime, playerAttackHitbox);
        }
    }
}

void Level::Draw(SpriteRenderer& renderer, double deltatime)
{
    for (auto& actor : Actors)
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

void Level::Init(std::vector<std::vector<int> >& tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    // calculate dimensions
    int height = static_cast<int>(tileData.size());
    int width = static_cast<int>(tileData[0].size()); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / static_cast<float>(height);

    // initialize level tiles based on tileData
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) // destroyable
            {
                glm::vec3 pos(unit_width * x, unit_height * y, 0.0f);
                glm::vec3 size(unit_width, unit_height, 0.0f);
                Sprite* blockSprite = new Sprite("block_solid");
                std::unique_ptr<Actor> blockActor = std::make_unique<Block> (
                    pos, size, blockSprite, glm::vec3(0.8f, 0.8f, 0.7f)
                );
                blockActor->IsDestroyable = true;
                Actors.push_back(std::move(blockActor));
            }
            else if (tileData[y][x] > 1)	// non-destroyable; now determine its color based on level data
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec3 pos(unit_width * x, unit_height * y, 0.0f);
                glm::vec3 size(unit_width, unit_height, 0.0f);
                Sprite* blockSprite = new Sprite("block");
                std::unique_ptr<Actor> blockActor = std::make_unique<Block> (
                    pos, size, blockSprite, color
                );
                Actors.push_back(std::move(blockActor));
            }
        }
    }

    InitEnemies();
}

void Level::InitEnemies()
{
    const glm::vec3 ENEMY_SIZE(16.0f, 9.0f, 0.0f);
    const glm::vec3 enemyPos = glm::vec3(700.0f, 200.0f, 0.0f);
    glm::vec3 charScale(1.0f, 1.0f, 1.0f);
    charScale.x = Config::Get()->GetValue(SRC_WIDTH) / ENEMY_SIZE.x;
    charScale.y = Config::Get()->GetValue(SRC_HEIGHT) / ENEMY_SIZE.y;

    Sprite* spikeEnemySprite = new Sprite("spike_enemy");
    float framePeriod = 0.06f;
    spikeEnemySprite->AddAnimation("spike_enemy_idle", AnimationType::IDLE, framePeriod);

    std::unique_ptr<Actor> enemy = std::make_unique<SpikeEnemy> (
        enemyPos, charScale, spikeEnemySprite, framePeriod
    );
    Actors.push_back(std::move(enemy));
}

void Level::RemoveFromLevel(std::unique_ptr<Actor> &actor)
{
    auto i = Actors.begin();
    auto end = Actors.end();
    while (i != end)
    {
        if (actor == *i) 
            i = Actors.erase(i);
        else
            ++i;
    }
}
