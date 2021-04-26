#pragma once

#include <vector>

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
    virtual ~Level();

    // loads level from file
    void Load(std::string file, unsigned int levelWidth, unsigned int levelHeight);
    
    // render level
    void Draw(SpriteRenderer& renderer, double deltatime);

private:

    // level state
    std::vector<Actor*> Actors;

    // initialize level from tile data
    void Init(std::vector<std::vector<int> >& tileData, unsigned int levelWidth, unsigned int levelHeight);
};
