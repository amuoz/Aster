#include "Level.h"

#include <fstream>
#include <sstream>

#include <glm/glm.hpp>

#include "Block.h"
#include "ResourceManager.h"

Level::Level()
{

}

Level::~Level()
{

}

void Level::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    Actors.clear();

    // load from file
    unsigned int tileCode;
    //Level level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
            {
                row.push_back(tileCode);
            }
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
        {
            Init(tileData, levelWidth, levelHeight);
        }
    }
}

void Level::Draw(SpriteRenderer& renderer)
{
    for (Actor* actor : Actors)
    {
        if (!actor->bDestroyed)
        {
            actor->Draw(renderer);
        }
    }
}

void Level::Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    // calculate dimensions
    unsigned int height = static_cast<unsigned int>(tileData.size());
    unsigned int width = static_cast<unsigned int>(tileData[0].size()); // note we can index vector at [0] since this function is only called if height > 0
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
                Block* blockActor = new Block(pos, size, ResourceManager::GetInstance()->GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
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
                Actors.push_back(new Block(pos, size, ResourceManager::GetInstance()->GetTexture("block"), color));
            }
        }
    }
}
