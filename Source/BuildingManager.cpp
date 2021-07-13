#include "BuildingManager.h"

#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include <algorithm>

#include "TileBuilder.h"
#include "Config.h"
#include "Sprite.h"
#include "Common.h"
#include "Entity/Actor.h"
#include "Entity/Building.h"

using namespace std;

BuildingManager::BuildingManager(shared_ptr<TileBuilder> tileBuilder)
{
    NumOfTilesX = 0;
    NumOfTilesY = 0;
    TileWidth = 0;
    TileHeight = 0;
    MapBuilder = tileBuilder;
}

BuildingManager::~BuildingManager()
{
}

void BuildingManager::SetLevelSize(int widthInTiles, int heightInTiles)
{
    NumOfTilesX = widthInTiles;
    NumOfTilesY = heightInTiles;
    TileWidth = Config::Get()->GetValue(LVL_WIDTH) / static_cast<float>(NumOfTilesX);
    TileHeight = Config::Get()->GetValue(LVL_HEIGHT) / static_cast<float>(NumOfTilesY);
}

shared_ptr<Actor> BuildingManager::CreateBuilding(tuple<int, int> doorCoords,
                                                  vector<vector<ActorType> > &actorTypes,
                                                  vector<vector<ActorType> > &interiorActorTypes,
                                                  vector<vector<int> > tiles,
                                                  vector<vector<int> > interiorTiles)
{
    vector<vector<ActorType> > buildingActorsTypes(NumOfTilesY,
                                                   vector<ActorType>(NumOfTilesX, ActorType::NONE));
    vector<vector<ActorType> > buildingInteriorActorsTypes(NumOfTilesY,
                                                           vector<ActorType>(NumOfTilesX, ActorType::NONE));
    vector<tuple<int, int, bool> > knownPositions;
    knownPositions.push_back(make_tuple(get<0>(doorCoords), get<1>(doorCoords), true));

    PreprocessBuildingActors(doorCoords,
                             buildingActorsTypes, buildingInteriorActorsTypes,
                             actorTypes, interiorActorTypes,
                             knownPositions);

    list<shared_ptr<Actor> > buildingActors;
    list<shared_ptr<Actor> > buildingInteriorActors;
    MapBuilder->CreateActors(buildingActors, buildingActorsTypes, tiles);
    MapBuilder->CreateActors(buildingInteriorActors, buildingInteriorActorsTypes, interiorTiles);

    glm::vec2 doorPosition(TileWidth * get<0>(doorCoords), TileHeight * get<1>(doorCoords));
    glm::vec3 doorSize(TileWidth, TileHeight, 0.0f);
    auto doorSprite = make_unique<Sprite>("door");
    auto floorSprite = make_unique<Sprite>("block");
    glm::vec4 floorColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    shared_ptr<Actor> building = make_shared<Building>(
        doorPosition, doorSize, move(doorSprite),
        move(floorSprite), floorColor,
        buildingActors, buildingInteriorActors);

    return building;
}

void BuildingManager::PreprocessBuildingActors(tuple<int, int> doorCoords,
                                               vector<vector<ActorType> > &buildingActorsTypes,
                                               vector<vector<ActorType> > &buildingInteriorActorsTypes,
                                               vector<vector<ActorType> > &actorTypes,
                                               vector<vector<ActorType> > &interiorActorTypes,
                                               vector<tuple<int, int, bool> > &knownPositions)
{
    tuple<int, int> currentCoords = make_tuple(
        get<0>(doorCoords),
        get<1>(doorCoords));

    bool isBuildingProcessed = false;

    while (!isBuildingProcessed)
    {
        int x = get<0>(currentCoords);
        int y = get<1>(currentCoords);
        buildingActorsTypes[y][x] = actorTypes[y][x];
        buildingInteriorActorsTypes[y][x] = interiorActorTypes[y][x];
        actorTypes[y][x] = ActorType::NONE;
        interiorActorTypes[y][x] = ActorType::NONE;

        // DebugBuildingPath(buildingActorsTypes, x, y);

        tuple<int, int, bool> nextTileCoords = GetNextTileCoords(x, y, actorTypes, knownPositions, 0);

        get<0>(currentCoords) = get<0>(nextTileCoords);
        get<1>(currentCoords) = get<1>(nextTileCoords);

        if (currentCoords == NO_TILE_COORDS)
        {
            isBuildingProcessed = true;
        }
    }
}

/**
 * Tile traversing algorithm for buildings.
 *
 * Returns the next tile for a building to store in the list of building actors.
 *
 * @param x
 * @param y Coords of the current building tile (starting at the door).
 * @param grid Matrix with all actor types in the level by coords
 * @param knownPositions Already visited tiles by this algorithm
 * @param directionIndex direction to test from current coords for the next tile
 * 
 * @return tuple with the coords for the next building tile to be stored
 *         or
 *         tuple with (-1, -1) to indicate end of the building.
 */
tuple<int, int, bool> BuildingManager::GetNextTileCoords(int x,
                                                         int y,
                                                         vector<vector<ActorType> > grid,
                                                         vector<tuple<int, int, bool> > &knownPositions,
                                                         int directionIndex)
{
    // next coords to test (x, y, shouldContinueTestingDirections)
    auto nextCoords = make_tuple(x + DIRECTIONS[directionIndex][0],
                                 y + DIRECTIONS[directionIndex][1],
                                 true);
    auto nextX = get<0>(nextCoords);
    auto nextY = get<1>(nextCoords);
    ActorType tileActor = grid[nextY][nextX];

    // Debug next tile
    // cout << "(" << nextX << ", " << nextY << ") ";

    auto foundPosition = FindKnownPosition(nextCoords, knownPositions);

    // if next tile is unknown
    if (foundPosition == knownPositions.end())
    {
        if (tileActor == ActorType::BLOCK || tileActor == ActorType::DOOR)
        {
            knownPositions.push_back(nextCoords);
            return nextCoords;
        }
        else
        {
            // if not block or door, don't test any direction from this tile
            get<2>(nextCoords) = false;
            knownPositions.push_back(nextCoords);
        }
    }

    // if not all directions tested, test next direction
    if (directionIndex < 3)
    {
        return GetNextTileCoords(x, y, grid, knownPositions, ++directionIndex);
    }
    // if all directions tested, test new directions of previous tiles
    else
    {
        int tileIndex;
        if (foundPosition == knownPositions.end())
        {
            tileIndex = knownPositions.size() - 1;
        }
        else
        {
            tileIndex = foundPosition - knownPositions.begin();
        }

        // if at the origin, finish building actors
        if (tileIndex == 0)
        {
            return NO_TILE_COORDS;
        }
        else
        {
            SetAllDirectionsTestedForLastBlockTile(foundPosition, directionIndex, knownPositions);
            auto previousPosition = GetPreviousPosition(knownPositions, tileIndex);
            return GetNextTileCoords(get<0>(previousPosition), get<1>(previousPosition), grid, knownPositions, 0);
        }
    }
}

vector<tuple<int, int, bool> >::iterator BuildingManager::FindKnownPosition(
    tuple<int, int, bool> &nextCoords,
    vector<tuple<int, int, bool> > &knownPositions)
{
    auto areNextCoordsKnown = [&nextCoords](const auto position)
    {
        return get<0>(position) == get<0>(nextCoords) &&
               get<1>(position) == get<1>(nextCoords);
    };

    return find_if(knownPositions.begin(), knownPositions.end(), areNextCoordsKnown);
}

void BuildingManager::SetAllDirectionsTestedForLastBlockTile(
    vector<tuple<int, int, bool> >::iterator &foundPosition,
    int directionIndex,
    vector<tuple<int, int, bool> > &knownPositions)
{
    auto directions = DIRECTIONS;
    auto isLastBlock = [&foundPosition, directionIndex, directions](const auto position)
    {
        return get<0>(position) == get<0>(*foundPosition) - directions[directionIndex][0] &&
               get<1>(position) == get<1>(*foundPosition) - directions[directionIndex][1];
    };
    auto lastBlockPosition = find_if(knownPositions.begin(), knownPositions.end(), isLastBlock);

    get<2>(*lastBlockPosition) = false;
}

tuple<int, int, bool> BuildingManager::GetPreviousPosition(vector<tuple<int, int, bool> > &knownPositions,
                                                           int tileIndex)
{
    tuple<int, int, bool> previousPosition;
    do
    {
        --tileIndex;
        previousPosition = knownPositions[tileIndex];
    } while (get<2>(previousPosition) == false && tileIndex > 0);

    return previousPosition;
}

void BuildingManager::DebugBuildingPath(vector<vector<ActorType> > buildingActorsTypes, int x, int y)
{
    int i = 0;
    int j = 0;
    for (auto line : buildingActorsTypes)
    {
        for (auto cell : line)
        {
            string symbol;
            if (x == i && y == j)
                symbol = "o";
            else if (cell == ActorType::NONE)
                symbol = ".";
            else
                symbol = "x";
            cout << symbol << " ";

            i++;
        }

        cout << endl;

        j++;
        i = 0;
    }
    cout << "//////////////////////" << endl;
}
