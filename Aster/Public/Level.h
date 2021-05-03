#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <list>
#include <vector>
#include <utility>

class Actor;
class SpriteRenderer;

/* Level holds all Tiles as part of a level and 
 * hosts functionality to Load/render levels from the harddisk.
 */
class Level
{
public:
    // constructor
    Level();
    ~Level();

    // loads level from file
    void Load(std::string file, unsigned int levelWidth, unsigned int levelHeight);

    void Update(float deltaTime, glm::vec4 playerAttackHitbox);

    // render level
    void Draw(SpriteRenderer &renderer, double deltatime);

    glm::vec3 GetPlayerPosition();

private:
    nlohmann::json LevelInfo;
    glm::vec3 PlayerPosition;
    std::vector<std::vector<int> > Tiles;

    std::list<std::unique_ptr<Actor> > Actors;

    void LoadTiles();
    void Init(unsigned int levelWidth, unsigned int levelHeight);
    void InitBlocks(unsigned int levelWidth, unsigned int levelHeight);
    void InitEnemies();
    void InitSpike(nlohmann::json  &enemyInfo);
    void RemoveFromLevel(std::unique_ptr<Actor> &actor);
};
