#include "TileBuilder.h"

#include <glm/glm.hpp>
#include <list>
#include <string>
#include <vector>

#include "Config.h"
#include "Sprite.h"
#include "Common.h"
#include "Entity/Actor.h"
#include "Entity/Block.h"
#include "Entity/Door.h"

using namespace std;

TileBuilder::TileBuilder()
{
  NumOfTilesX = 0;
  NumOfTilesY = 0;
  TileWidth = 0;
  TileHeight = 0;
}

TileBuilder::~TileBuilder()
{
}

void TileBuilder::SetLevelSize(int widthInTiles, int heightInTiles)
{
  NumOfTilesX = widthInTiles;
  NumOfTilesY = heightInTiles;
  TileWidth = Config::Get()->GetValue(LVL_WIDTH) / static_cast<float>(NumOfTilesX);
  TileHeight = Config::Get()->GetValue(LVL_HEIGHT) / static_cast<float>(NumOfTilesY);
}

void TileBuilder::CreateActors(list<shared_ptr<Actor> > &actors,
                               vector<vector<ActorType> > actorTypes,
                               vector<vector<int> > tiles)
{
  for (int y = 0; y < NumOfTilesY; ++y)
  {
    for (int x = 0; x < NumOfTilesX; ++x)
    {
      // check block type from level data (2D level array)
      if (actorTypes[y][x] == ActorType::DESTROYABLE_BLOCK)
      {
        shared_ptr<Actor> blockActor = CreateDestroyableBlock(x, y);
        actors.push_back(blockActor);
      }
      else if (actorTypes[y][x] == ActorType::BLOCK)
      {
        shared_ptr<Actor> blockActor = CreateBlock(x, y, tiles);
        actors.push_back(blockActor);
      }
      else if (actorTypes[y][x] == ActorType::DOOR)
      {
        shared_ptr<Actor> doorActor = CreateDoor(x, y);
        actors.push_back(doorActor);
      }
    }
  }
}

shared_ptr<Actor> TileBuilder::CreateDestroyableBlock(int x, int y)
{
  glm::vec2 pos(TileWidth * x, TileHeight * y);
  glm::vec3 size(TileWidth, TileHeight, 0.0f);
  auto blockSprite = make_unique<Sprite>("block");
  glm::vec4 color = glm::vec4(0.9f, 0.9f, 1.0f, 1.0f);
  shared_ptr<Actor> blockActor = make_shared<Block>(
      pos, size, move(blockSprite), color);
  blockActor->IsDestroyable = true;

  return blockActor;
}

shared_ptr<Actor> TileBuilder::CreateBlock(int x, int y, vector<vector<int> > tiles)
{
  glm::vec2 pos(TileWidth * x, TileHeight * y);
  glm::vec3 size(TileWidth, TileHeight, 0.0f);
  BlockLocation location = GetBlockLocation(x, y, tiles);
  auto blockSprite = GetBlockSprite(location);
  glm::vec4 color = glm::vec4(1.0f);
  shared_ptr<Actor> blockActor = make_shared<Block>(
      pos, size, move(blockSprite), color, location);

  return blockActor;
}

shared_ptr<Actor> TileBuilder::CreateDoor(int x, int y)
{
  glm::vec2 pos(TileWidth * x, TileHeight * y);
  glm::vec3 size(TileWidth, TileHeight, 0.0f);
  auto blockSprite = make_unique<Sprite>("door");
  glm::vec4 color = glm::vec4(1.0f);
  shared_ptr<Actor> doorActor = make_shared<Door>(
      pos, size, move(blockSprite), color);

  return doorActor;
}

BlockLocation TileBuilder::GetBlockLocation(int x, int y, vector<vector<int> > tiles)
{
  int top, bottom, left, right;

  if (y == 0)
  {
    TopBlocks(top, bottom, left, right, x, y, tiles);
  }
  else if (y == tiles.size() - 1)
  {
    BottomBlocks(top, bottom, left, right, x, y, tiles);
  }
  else
  {
    MiddleBlocks(top, bottom, left, right, x, y, tiles);
  }

  return GetBlockLocationByNeighbors(top, bottom, left, right);
}

void TileBuilder::TopBlocks(int &top, int &bottom, int &left, int &right, int x, int y, vector<vector<int> > tiles)
{
  if (x == 0)
  // left blocks
  {
    left = 2;
    right = tiles[0][1];
  }
  else if (x == tiles[0].size() - 1)
  // right blocks
  {
    left = tiles[0][x - 1];
    right = 2;
  }
  else
  // middle blocks
  {
    left = tiles[0][x - 1];
    right = tiles[0][x + 1];
  }

  top = 2;
  bottom = tiles[y + 1][x];
}

void TileBuilder::BottomBlocks(int &top, int &bottom, int &left, int &right, int x, int y, vector<vector<int> > tiles)
{
  if (x == 0)
  // left blocks
  {
    left = 2;
    right = tiles[x][y + 1];
  }
  else if (x == tiles[y].size() - 1)
  // right blocks
  {
    left = tiles[y][x - 1];
    right = 2;
  }
  else
  // middle blocks
  {
    left = tiles[y][x - 1];
    right = tiles[y][x + 1];
  }

  top = tiles[y - 1][x];
  bottom = 2;
}

void TileBuilder::MiddleBlocks(int &top, int &bottom, int &left, int &right, int x, int y, vector<vector<int> > tiles)
{
  if (x == 0)
  // left blocks
  {
    left = 2;
    right = tiles[y][x + 1];
  }
  else if (x == tiles[y].size() - 1)
  // right blocks
  {
    left = tiles[y][x - 1];
    right = 2;
  }
  else
  // middle blocks
  {
    left = tiles[y][x - 1];
    right = tiles[y][x + 1];
  }

  top = tiles[y - 1][x];
  bottom = tiles[y + 1][x];
}

BlockLocation TileBuilder::GetBlockLocationByNeighbors(int top, int bottom, int left, int right)
{
  if (bottom < 2)
  {
    if (left < 2)
    {
      return BlockLocation::BOTTOM_LEFT;
    }
    if (right < 2)
    {
      return BlockLocation::BOTTOM_RIGHT;
    }

    return BlockLocation::BOTTOM;
  }

  if (top < 2)
  {
    if (left < 2)
    {
      return BlockLocation::TOP_LEFT;
    }
    if (right < 2)
    {
      return BlockLocation::TOP_RIGHT;
    }

    return BlockLocation::TOP;
  }

  if (left < 2)
  {
    return BlockLocation::LEFT;
  }
  if (right < 2)
  {
    return BlockLocation::RIGHT;
  }

  return BlockLocation::MIDDLE;
}

unique_ptr<Sprite> TileBuilder::GetBlockSprite(BlockLocation location)
{
  string spriteName = BLOCK_SPRITES[location];
  return make_unique<Sprite>(spriteName);
}
