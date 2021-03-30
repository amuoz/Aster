#include "Block.h"

Block::Block(): Actor()
{

}

Block::Block(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color, glm::vec3 velocity): Actor(pos, size, sprite, color, velocity)
{

}

Block::~Block()
{

}

void Block::Render(Shader)
{

}

void Block::Update(float)
{

}
