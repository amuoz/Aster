#include "Block.h"

#include "Common.h"
#include "Sprite.h"
#include "Physics.h"
#include "PhysicActor.h"

Block::Block(glm::vec3 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color, glm::vec3 velocity): Actor(pos, size, std::move(sprite), color, velocity)
{
	ActorCollider = g_PhysicsPtr->AddDynamicActor(pos, velocity, size, false, CollisionChannel::STATIC, glm::vec3(0.0f), 1.0f);
	ActorCollider->report = this;
}

Block::~Block()
{

}

void Block::Update(float, glm::vec4 attackHitbox)
{
	if (IsDestroyable && IsAttacked(attackHitbox)) {
		IsDestroyed = true;
	}
}
