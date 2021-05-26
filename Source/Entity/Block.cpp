#include "Block.h"

#include "Sprite.h"
#include "Physics.h"
#include "PhysicActor.h"
#include <iostream>

//  PhysicsActor pos & size
//
//               SIZE_RATIO_X
//              <------->
//   __________________  ^
//  |           |      | |
//  |   Physics Actor  | | SIZE_RATIO_Y
//  |           |      | |
//  |           |______| v
//  |                  |
//  |   Actor Sprite   |
//  |__________________|

const float SIZE_RATIO_X = 0.33f;
const float SIZE_RATIO_Y = 0.5f;

Block::Block(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color, BlockLocation location): Actor(pos, size, std::move(sprite), color)
{
	Location = location;
	ZIndex = 0.9f;

	if (!IsTopLocation())
	{
		glm::vec2 physicsPosition = GetPhysicsPosition(pos, size, location);
		glm::vec3 physicsSize = GetPhysicsSize(size, location);
		ActorCollider = Physics::Get()->AddDynamicActor(physicsPosition, physicsSize, CollisionChannel::STATIC);
		ZIndex = 0.1f;
	}
}

Block::~Block()
{
	//std::cout << "Block destroyed" << std::endl;
}

void Block::Update(float, glm::vec4 attackHitbox)
{
	if (IsDestroyable && IsAttacked(attackHitbox)) {
		IsDestroyed = true;
	}
}

glm::vec2 Block::GetPhysicsPosition(glm::vec2 pos, glm::vec3 size, BlockLocation location)
{
	switch (location)
	{
	case BlockLocation::LEFT:
		return glm::vec2(pos.x + size.x * (1 - SIZE_RATIO_X), pos.y);
	case BlockLocation::MIDDLE:
	case BlockLocation::RIGHT:
	default:
		return pos;
	case BlockLocation::BOTTOM_LEFT:
		return glm::vec2(pos.x + size.x * (1 - SIZE_RATIO_X), pos.y - size.y * (1 - SIZE_RATIO_Y));
	case BlockLocation::BOTTOM:
	case BlockLocation::BOTTOM_RIGHT:
		return glm::vec2(pos.x, pos.y - size.y * (1 - SIZE_RATIO_Y));
	}
}

glm::vec3 Block::GetPhysicsSize(glm::vec3 size, BlockLocation location)
{
	switch (location)
	{
	case BlockLocation::LEFT:
	case BlockLocation::RIGHT:
		return glm::vec3(size.x * SIZE_RATIO_X, size.y, size.z);
	case BlockLocation::MIDDLE:
	default:
		return size;
	case BlockLocation::BOTTOM_LEFT:
	case BlockLocation::BOTTOM_RIGHT:
		return glm::vec3(size.x * SIZE_RATIO_X, size.y * SIZE_RATIO_Y, size.z);
	case BlockLocation::BOTTOM:
		return glm::vec3(size.x, size.y * SIZE_RATIO_Y, size.z);
	}
}

bool Block::IsTopLocation()
{
	return Location == BlockLocation::TOP ||
				 Location == BlockLocation::TOP_RIGHT ||
				 Location == BlockLocation::TOP_LEFT;
}
