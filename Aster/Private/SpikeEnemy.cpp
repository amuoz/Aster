#include "SpikeEnemy.h"

#include "Common.h"
#include "Sprite.h"

#include "SpriteRenderer.h"

SpikeEnemy::SpikeEnemy(glm::vec3 pos, glm::vec3 size, Sprite *sprite, glm::vec3 color, glm::vec3 velocity) : Actor(pos, size, sprite, color, velocity)
{
	m_physicsActor = g_PhysicsPtr->AddDynamicActor(pos, velocity, size, glm::vec3(0.0f), 1.0f);
	m_physicsActor->bCheckCollision = true;
	m_physicsActor->report = this;
	State = ActorState::IDLE;
	LastState = ActorState::IDLE;
}

SpikeEnemy::~SpikeEnemy()
{
}

void SpikeEnemy::Render(Shader)
{
}

void SpikeEnemy::Update(float, glm::vec4)
{
}

void SpikeEnemy::Draw(SpriteRenderer &renderer, double deltatime)
{
	switch (State)
	{
	case ActorState::IDLE:
	default:
		CurrentAnimation = AnimationType::IDLE;
		break;
	}

	m_sprite->Draw(CurrentAnimation, renderer, deltatime, m_position, m_scale, m_rotAngle, m_color);
}

void SpikeEnemy::SetState(ActorState state)
{
	if (State != state)
	{
		LastState = State;
	}

	State = state;
}

void SpikeEnemy::OnContact(Physics::PhysicActor *physicActor)
{
	// resolved collision gives corrected position
	m_position = m_physicsActor->pos;
}
