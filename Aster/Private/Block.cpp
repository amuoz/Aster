#include "Block.h"

#include "Common.h"
#include "Sprite.h"

Block::Block(glm::vec3 pos, glm::vec3 size, Sprite* sprite, glm::vec3 color, glm::vec3 velocity): Actor(pos, size, sprite, color, velocity)
{
	m_physicsActor = g_PhysicsPtr->AddDynamicActor(pos, velocity, size, glm::vec3(0.0f), 1.0f);
	m_physicsActor->report = this;
}

Block::~Block()
{

}

void Block::Render(Shader)
{

}

void Block::Update(float, glm::vec4 attackHitbox)
{
	if (IsDestroyable && IsAttacked(attackHitbox)) {
		IsDestroyed = true;
	}
}

void Block::TakeDamage()
{
}

void Block::OnContact(Physics::PhysicActor* physicActor)
{
	//std::cout << "...Block OnContact..." << std::endl;
}
