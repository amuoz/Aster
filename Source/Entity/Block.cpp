#include "Block.h"

#include "Sprite.h"
#include "Physics.h"
#include "PhysicActor.h"
#include <iostream>

Block::Block(glm::vec3 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color): Actor(pos, size, std::move(sprite), color)
{
	ActorCollider = Physics::Get()->AddDynamicActor(pos, size, CollisionChannel::STATIC);
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
