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

    std::map<BlockLocation, std::string> BLOCK_SPRITES = {
        {BlockLocation::BOTTOM_LEFT, "block_corner_left"},
        {BlockLocation::BOTTOM_RIGHT, "block_corner_right"},
        {BlockLocation::BOTTOM, "block_bottom"},
        {BlockLocation::LEFT, "block_side_left"},
        {BlockLocation::RIGHT, "block_side_right"},
        {BlockLocation::MIDDLE, "block"},
        {BlockLocation::TOP_LEFT, "block_top_left"},
        {BlockLocation::TOP_RIGHT, "block_top_right"},
        {BlockLocation::TOP, "block_top"}};

    std::map<std::string, PowerUpType> POWER_UPS = {
        {"sword_powerup", PowerUpType::SWORD},
        {"spear_powerup", PowerUpType::SPEAR},
        {"hammer_powerup", PowerUpType::HAMMER}};

    enum class ActorType
    {
        NONE,
        DESTROYABLE_BLOCK,
        BLOCK,
        DOOR
    };

    std::vector<std::vector<int> > DIRECTIONS = {
        {-1, 0},
        {0, -1},
        {1, 0},
        {0, 1}};

    void LoadTiles();
    void InitBlocks(unsigned int levelWidth, unsigned int levelHeight);
    void InitEnemies();
    void InitSpike(nlohmann::json &enemyInfo);
    void InitPowerUps();
    void InitPowerUp(std::string name, nlohmann::json &info);

    void CreatePlayer(glm::vec2 playerPosition);
    glm::vec2 GetPlayerPosition();
    BlockLocation GetBlockLocation(int x, int y);
    BlockLocation GetBlockLocationByNeighbors(int top, int bottom, int left, int right);
    std::unique_ptr<Sprite> GetBlockSprite(BlockLocation location);
    void TopBlocks(int &top, int &bottom, int &left, int &right, int x, int y);
    void BottomBlocks(int &top, int &bottom, int &left, int &right, int x, int y);
    void MiddleBlocks(int &top, int &bottom, int &left, int &right, int x, int y);
    void CreateActors(std::vector<std::vector<ActorType> > actorTypes,
                      std::list<std::shared_ptr<Actor> > &actors,
                      unsigned int levelWidth,
                      unsigned int levelHeight);
    std::shared_ptr<Actor> CreateBuilding(std::tuple<int, int> coords,
                                          std::vector<std::vector<ActorType> > &actorTypes,
                                          unsigned int levelWidth,
                                          unsigned int levelHeight);
    std::tuple<int, int, bool> GetNextTileCoords(int x,
                                           int y,
                                           std::vector<std::vector<ActorType> > grid,
                                           std::vector<std::tuple<int, int, bool> > &knownPositions,
                                           int directionIndex);
    std::shared_ptr<Actor> CreateDestroyableBlock(float unit_width, float unit_height, int x, int y);
    std::shared_ptr<Actor> CreateBlock(float unit_width, float unit_height, int x, int y);
    std::shared_ptr<Actor> CreateDoor(float unit_width, float unit_height, int x, int y);
    void DebugBuildingPath(std::vector<std::vector<ActorType> > buildingActorsTypes, int x, int y);
};
