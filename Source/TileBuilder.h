#pragma once

#include <glm/glm.hpp>
#include <list>
#include <string>
#include <vector>
#include <map>

#include "AsterTypes.h"

using namespace std;

class Actor;
class Sprite;

class TileBuilder
{
public:
    TileBuilder();
    ~TileBuilder();

    void SetLevelSize(int widthInTiles, int heightInTiles);
    void CreateActors(list<shared_ptr<Actor> > &actors,
                      vector<vector<ActorType> > actorTypes,
                      vector<vector<int> > tiles);

private:
    int NumOfTilesX;
    int NumOfTilesY;
    float TileWidth;
    float TileHeight;

    map<BlockLocation, string> BLOCK_SPRITES = {
        {BlockLocation::BOTTOM_LEFT, "block_corner_left"},
        {BlockLocation::BOTTOM_RIGHT, "block_corner_right"},
        {BlockLocation::BOTTOM, "block_bottom"},
        {BlockLocation::LEFT, "block_side_left"},
        {BlockLocation::RIGHT, "block_side_right"},
        {BlockLocation::MIDDLE, "block"},
        {BlockLocation::TOP_LEFT, "block_top_left"},
        {BlockLocation::TOP_RIGHT, "block_top_right"},
        {BlockLocation::TOP, "block_top"}};

    shared_ptr<Actor> CreateDestroyableBlock(int x, int y);
    shared_ptr<Actor> CreateBlock(int x, int y, vector<vector<int> > tiles);
    shared_ptr<Actor> CreateDoor(int x, int y);
    shared_ptr<Actor> CreateFloor(int x, int y);

    BlockLocation GetBlockLocation(int x, int y, vector<vector<int> > tiles);
    void TopBlocks(int &top, int &bottom, int &left, int &right, int x, int y, vector<vector<int> > tiles);
    void BottomBlocks(int &top, int &bottom, int &left, int &right, int x, int y, vector<vector<int> > tiles);
    void MiddleBlocks(int &top, int &bottom, int &left, int &right, int x, int y, vector<vector<int> > tiles);
    BlockLocation GetBlockLocationByNeighbors(int top, int bottom, int left, int right);
    unique_ptr<Sprite> GetBlockSprite(BlockLocation location);
};
