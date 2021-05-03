#include "SpikeEnemy.h"

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Common.h"
#include "Sprite.h"
#include "Player.h"
#include "Physics.h"
#include "SpriteRenderer.h"

// TODO: calculate this vvv based on the current animation
const int ANIMATION_FRAMES = 18;
const float STILL_CHANCE = 0.3;
const float CHANGE_DIRECTION_CHANCE = 0.2;
const float AGGRO_SIZE = 350;

SpikeEnemy::SpikeEnemy(glm::vec3 pos, glm::vec3 size, Sprite *sprite, float framePeriod, glm::vec3 color, glm::vec3 velocity) : Actor(pos, size, sprite, color, velocity)
{
	ActorCollider = g_PhysicsPtr->AddDynamicActor(
			pos,
			velocity,
			size,
			false,
			glm::vec3(0.0f),
			1.0f);
	ActorCollider->bCheckCollision = true;
	ActorCollider->report = this;

	AggroCollider = g_PhysicsPtr->AddDynamicActor(
			GetAggroPosition(pos, size),
			velocity,
			glm::vec3(AGGRO_SIZE, AGGRO_SIZE, 0),
			true,
			glm::vec3(0.0f),
			1.0f);
	AggroCollider->bCheckCollision = true;
	AggroCollider->report = this;

	AnimationPeriod = framePeriod * ANIMATION_FRAMES;
	AnimationProgress = 0.0f;
	State = ActorState::IDLE;
	LastState = ActorState::IDLE;
	Direction = glm::vec3(0, 0, 0);
	Speed = MAX_SPEED;
	ChangeDirectionPeriod = 1.0f;
	LastDirectionChange = 0.0f;
	srand(time(NULL));
}

SpikeEnemy::~SpikeEnemy()
{
	AggroCollider->report = nullptr;
}

void SpikeEnemy::Render(Shader)
{
}

void SpikeEnemy::Update(float deltaTime, glm::vec4 attackHitbox)
{
	if (IsAttacked(attackHitbox))
	{
		IsDestroyed = true;
	}

	if (AggroCollider->Collisions.size() == 0)
		SetState(ActorState::IDLE);
	else
	{
		auto it = std::find_if(
			AggroCollider->Collisions.begin(),
			AggroCollider->Collisions.end(),
			[](const auto &dynamicActor) {
				return dynamicActor->report && dynamicActor->report->IsPlayer();
			});
		if (it == AggroCollider->Collisions.end())
			SetState(ActorState::IDLE);
	}

	AnimationProgress += deltaTime;
	if (AnimationProgress > AnimationPeriod)
	{
		AnimationProgress -= AnimationPeriod;

		switch (State)
		{
		case ActorState::IDLE:
		default:
			SetWanderMovement();
			break;
		case ActorState::AGGRO:
			SetAggroMovement();
			break;
		}
	}

	SetSpeed();

	Move(deltaTime, Direction);

	AggroCollider->pos = GetAggroPosition(m_position, m_scale);
}

void SpikeEnemy::Draw(SpriteRenderer &renderer, double deltatime)
{
	switch (State)
	{
	case ActorState::IDLE:
		m_color = glm::vec3(1, 1, 1);
		CurrentAnimation = AnimationType::IDLE;
		break;
	case ActorState::AGGRO:
		m_color = glm::vec3(1, 0.5, 0.5);
		CurrentAnimation = AnimationType::IDLE;
		break;
	default:
		break;
	}

	m_sprite->Draw(
			CurrentAnimation,
			renderer,
			deltatime,
			m_position,
			m_scale,
			m_rotAngle,
			m_color);
}

void SpikeEnemy::TakeDamage()
{
}

void SpikeEnemy::OnContact(
		std::shared_ptr<Physics::PhysicActor> external,
		std::shared_ptr<Physics::PhysicActor> internal)
{
	if (internal == ActorCollider)
	{
		auto *collidedActor = external->report;
		collidedActor->TakeDamage();
	}

	if (internal == AggroCollider)
	{
		if (external->report->IsPlayer())
		{
			SetState(ActorState::AGGRO);
			ObjectivePosition = external->report->GetPosition();
		}
	}

	m_position = ActorCollider->pos;
}

bool SpikeEnemy::PassRandomChance(float chance)
{
	return (float(rand()) / float(RAND_MAX)) < chance;
}

float SpikeEnemy::GetRandomDirectionComponent()
{
	return (float(rand()) / float(RAND_MAX)) * 2 - 1;
}

glm::vec3 SpikeEnemy::GetRandomDirection()
{
	return glm::vec3(
			GetRandomDirectionComponent(),
			GetRandomDirectionComponent(),
			0);
}

void SpikeEnemy::SetWanderMovement()
{
	// 50% still
	if (PassRandomChance(STILL_CHANCE))
	{
		Direction = glm::vec3(0, 0, 0);
	}
	// 50% moving
	else
	{
		// 20% change direction
		if (PassRandomChance(CHANGE_DIRECTION_CHANCE))
		{
			Direction = GetRandomDirection();
		}
		// 80% maintain direction
	}
}

void SpikeEnemy::SetAggroMovement()
{
	Direction = glm::normalize(ObjectivePosition - m_position);
}

void SpikeEnemy::SetSpeed()
{
	if (
			AnimationProgress < AnimationPeriod / 5 || AnimationProgress > AnimationPeriod / 2)
	{
		Speed = MAX_SPEED / 5;
	}
	else
	{
		Speed = MAX_SPEED;
	}
}

glm::vec3 SpikeEnemy::GetAggroPosition(glm::vec3 actorPosition, glm::vec3 actorSize)
{
	float posCorrectionX = (AGGRO_SIZE - actorSize.x) / 2;
	float posCorrectionY = (AGGRO_SIZE - actorSize.y) / 2;

	return glm::vec3(
			actorPosition.x - posCorrectionX,
			actorPosition.y - posCorrectionY,
			0);
}
