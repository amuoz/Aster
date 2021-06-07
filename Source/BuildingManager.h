#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <tuple>

#include "AsterTypes.h"

using namespace std;

class Actor;
class TileBuilder;

class BuildingManager
{
public:
    BuildingManager(shared_ptr<TileBuilder> tileBuilder);
    ~BuildingManager();

    void SetLevelSize(int widthInTiles, int heightInTiles);

    shared_ptr<Actor> CreateBuilding(tuple<int, int> doorCoords,
                                     vector<vector<ActorType> > &actorTypes,
                                     vector<vector<int> > tiles);

private:
    vector<vector<int> > DIRECTIONS = {
        {-1, 0},
        {0, -1},
        {1, 0},
        {0, 1}};

    tuple<int, int, bool> NO_TILE_COORDS = make_tuple(-1, -1, false);

    int NumOfTilesX;
    int NumOfTilesY;
    float TileWidth;
    float TileHeight;
    shared_ptr<TileBuilder> MapBuilder;

    void PreprocessBuildingActor(tuple<int, int> coords,
                                 vector<vector<ActorType> > &buildingActorsTypes,
                                 vector<vector<ActorType> > &actorTypes,
                                 vector<tuple<int, int, bool> > &knownPositions);

    tuple<int, int, bool> GetNextTileCoords(int x,
                                            int y,
                                            vector<vector<ActorType> > grid,
                                            vector<tuple<int, int, bool> > &knownPositions,
                                            int directionIndex);

    vector<tuple<int, int, bool> >::iterator FindKnownPosition(tuple<int, int, bool> &nextCoords,
                                                               vector<tuple<int, int, bool> > &knownPositions);

    void SetAllDirectionsTestedForLastBlockTile(vector<tuple<int, int, bool> >::iterator &foundPosition,
                                                int directionIndex,
                                                vector<tuple<int, int, bool> > &knownPositions);

    tuple<int, int, bool> GetPreviousPosition(vector<tuple<int, int, bool> > &knownPositions,
                                              int tileIndex);

    void DebugBuildingPath(vector<vector<ActorType> > buildingActorsTypes, int x, int y);
};
