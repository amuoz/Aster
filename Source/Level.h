#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <list>
#include <vector>
#include <utility>

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

    std::shared_ptr<Player> GetPlayer();

private:
    nlohmann::json LevelInfo;
    std::vector<std::vector<int> > Tiles;
    std::shared_ptr<Player> Character;
    std::list<std::shared_ptr<Actor> > Actors;

    void LoadTiles();
    void InitBlocks(unsigned int levelWidth, unsigned int levelHeight);
    void InitEnemies();
    void InitSpike(nlohmann::json &enemyInfo);
    void InitPowerUps();
    void InitSword(nlohmann::json &powerUpInfo);
    void InitSpear(nlohmann::json &powerUpInfo);
    void InitHammer(nlohmann::json &powerUpInfo);
    
    void CreatePlayer(glm::vec3 playerPosition);
    glm::vec3 GetPlayerPosition();
    std::unique_ptr<Sprite> GetBlockSprite(int x, int y);
    std::unique_ptr<Sprite> GetBlockSpriteByPosition(int top, int bottom, int left, int right);
    void TopBlocks(int &top, int &bottom, int &left, int &right, int x, int y);
    void BottomBlocks(int &top, int &bottom, int &left, int &right, int x, int y);
    void MiddleBlocks(int &top, int &bottom, int &left, int &right, int x, int y);
};
