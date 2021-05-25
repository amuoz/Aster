#include "Block.h"

#include "Common.h"
#include "Sprite.h"
#include "Physics.h"
#include "PhysicActor.h"

//	PhysicsActor pos & size
//	____________________ ^
//	|									 | |
//	|		Actor Sprite   | | OFFSET_Y
//	|								   | |
//	|			 			_______| v
//	|			 			|			 |
//	|		 Physics Actor |
//	|___________|______|
//	<----------->
//		OFFSET_X

const float OFFSET_X = 0.66f;
const float OFFSET_Y = 0.5f;

Block::Block(glm::vec3 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color, BlockLocation location, glm::vec3 velocity) : Actor(pos, size, std::move(sprite), color, velocity)
{
	glm::vec3 physicsPosition = GetPhysicsPosition(pos, size, location);
	glm::vec3 physicsSize = GetPhysicsSize(size, location);
	ActorCollider = g_PhysicsPtr->AddDynamicActor(physicsPosition, velocity, physicsSize, false, CollisionChannel::STATIC, glm::vec3(0.0f), 1.0f);
	ActorCollider->report = this;
}

Block::~Block()
{
}

void Block::Update(float, glm::vec4 attackHitbox)
{
	if (IsDestroyable && IsAttacked(attackHitbox))
	{
		IsDestroyed = true;
	}
}

glm::vec3 Block::GetPhysicsPosition(glm::vec3 pos, glm::vec3 size, BlockLocation location)
{
	switch (location)
	{
	case BlockLocation::TOP_LEFT:
		return glm::vec3(pos.x + size.x * OFFSET_X, pos.y + size.y * OFFSET_Y, pos.z);
	case BlockLocation::TOP:
	case BlockLocation::TOP_RIGHT:
		return glm::vec3(pos.x, pos.y + size.y * OFFSET_Y, pos.z);
	case BlockLocation::LEFT:
	case BlockLocation::BOTTOM_LEFT:
		return glm::vec3(pos.x + size.x * OFFSET_X, pos.y, pos.z);
	case BlockLocation::MIDDLE:
	case BlockLocation::RIGHT:
	case BlockLocation::BOTTOM:
	case BlockLocation::BOTTOM_RIGHT:
	default:
		return pos;
	}
}

glm::vec3 Block::GetPhysicsSize(glm::vec3 size, BlockLocation location)
{
	switch (location)
	{
	case BlockLocation::TOP_LEFT:
	case BlockLocation::TOP_RIGHT:
	case BlockLocation::BOTTOM_LEFT:
	case BlockLocation::BOTTOM_RIGHT:
		return glm::vec3(size.x * (1.0f - OFFSET_X), size.y * OFFSET_Y, size.z);
	case BlockLocation::TOP:
	case BlockLocation::BOTTOM:
		return glm::vec3(size.x, size.y * OFFSET_Y, size.z);
	case BlockLocation::LEFT:
	case BlockLocation::RIGHT:
		return glm::vec3(size.x * (1.0f - OFFSET_X), size.y, size.z);
	case BlockLocation::MIDDLE:
	default:
		return size;
	}
}
