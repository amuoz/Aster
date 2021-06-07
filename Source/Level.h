#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <list>
#include <vector>
#include <utility>

#include "AsterTypes.h"

class Actor;
class SpriteRenderer;
class Player;
class Sprite;
class BuildingManager;
class TileBuilder;

/* Level holds all Tiles as part of a level and 
 * hosts functionality to Load/render levels from the harddisk.
 */
class Level
{
public:
    // constructor
    Level(std::shared_ptr<BuildingManager> buildingManager, std::shared_ptr<TileBuilder> tileBuilder);
    ~Level();

    // loads level from file
    void Load(std::string file);

    void Update(float deltaTime, glm::vec4 playerAttackHitbox);

    // render level
    void Draw(SpriteRenderer &renderer, double deltaTime);

    // BeginPlay event
    void BeginPlay();

    std::shared_ptr<Player> GetPlayer();

private:
    nlohmann::json LevelInfo;
    std::vector<std::vector<int> > Tiles;
    std::vector<std::vector<int> > InteriorTiles;
    std::shared_ptr<Player> Character;
    std::list<std::shared_ptr<Actor> > Actors;
    std::shared_ptr<BuildingManager> RoomsManager;
    std::shared_ptr<TileBuilder> MapBuilder;
    int NumOfTilesX;
    int NumOfTilesY;

    std::map<std::string, PowerUpType> POWER_UPS = {
        {"sword_powerup", PowerUpType::SWORD},
        {"spear_powerup", PowerUpType::SPEAR},
        {"hammer_powerup", PowerUpType::HAMMER}};

    void LoadTiles();
    void InitBlocks();
    void InitEnemies();
    void InitSpike(nlohmann::json &enemyInfo);
    void InitPowerUps();
    void InitPowerUp(std::string name, nlohmann::json &info);

    void CreatePlayer(glm::vec2 playerPosition);
    glm::vec2 GetPlayerPosition();
};
