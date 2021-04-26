#include "Level.h"

#include <glm/glm.hpp>
#include <vector>

#include "Block.h"
#include "ResourceManager.h"
#include "Files.h"
#include "Sprite.h"

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

void Level::Draw(SpriteRenderer& renderer, double deltatime)
{
    for (Actor* actor : Actors)
    {
        if (!actor->bDestroyed)
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
            if (tileData[y][x] == 1) // solid
            {
                glm::vec3 pos(unit_width * x, unit_height * y, 0.0f);
                glm::vec3 size(unit_width, unit_height, 0.0f);
                Sprite* blockSprite = new Sprite("block_solid");
                Block* blockActor = new Block(pos, size, blockSprite, glm::vec3(0.8f, 0.8f, 0.7f));
                blockActor->bIsSolid = true;
                Actors.push_back(blockActor);
            }
            else if (tileData[y][x] > 1)	// non-solid; now determine its color based on level data
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
                Actors.push_back(new Block(pos, size, blockSprite, color));
            }
        }
    }
}
