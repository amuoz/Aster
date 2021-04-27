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

void Block::Update(float, glm::vec4 playerAttackHitbox)
{
	if (IsDestroyable && IsAttacked(playerAttackHitbox)) {
		IsDestroyed = true;
	}
}

void Block::OnContact(Physics::PhysicActor* physicActor)
{
	//std::cout << "...Block OnContact..." << std::endl;
}

bool Block::IsAttacked(glm::vec4 playerAttackHitbox)
{
	float xHitbox = playerAttackHitbox.x;
	float yHitbox = playerAttackHitbox.y;
	float widthHitbox = playerAttackHitbox.z;
	float heightHitbox = playerAttackHitbox.w;

	float blockWidth = m_scale.x;
	float blockHeight = m_scale.y;

	bool xCollision = m_position.x + blockWidth >= xHitbox && xHitbox + widthHitbox >= m_position.x;
	
	bool yCollision = m_position.y + blockHeight >= yHitbox && yHitbox + heightHitbox >= m_position.y;

	return xCollision && yCollision;
}
