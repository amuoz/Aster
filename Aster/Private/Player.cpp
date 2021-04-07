#include "Player.h"
#include "Common.h"

Player::Player(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color, glm::vec3 velocity): Actor(pos, size, sprite, color, velocity)
{
	m_physicsActor = g_PhysicsPtr->AddDynamicActor(pos, velocity, size, glm::vec3(0.0f), 1.0f);
	m_physicsActor->bCheckCollision = true;
	m_physicsActor->report = this;
}

Player::~Player()
{

}

void Player::Render(Shader)
{

}

void Player::Update(float)
{

}

void Player::Move(float deltaTime, glm::vec3 direction)
{
	float velocity = PLAYER_VELOCITY * deltaTime;
	SetPosition(velocity * direction);
}

void Player::OnContact(Physics::PhysicActor* physicActor)
{
	// resolved collision gives corrected position
	m_position = m_physicsActor->pos;
}