#include "SpikeEnemy.h"

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Common.h"
#include "Sprite.h"
#include "Player.h"

#include "SpriteRenderer.h"

// TODO: calculate this vvv based on the current animation
const int ANIMATION_FRAMES = 18;
const float STILL_CHANCE = 0.3;
const float CHANGE_DIRECTION_CHANCE = 0.2;

SpikeEnemy::SpikeEnemy(glm::vec3 pos, glm::vec3 size, Sprite *sprite, float framePeriod, glm::vec3 color, glm::vec3 velocity) : Actor(pos, size, sprite, color, velocity)
{
	ActorCollider = g_PhysicsPtr->AddDynamicActor(pos, velocity, size, false, glm::vec3(0.0f), 1.0f);
	ActorCollider->bCheckCollision = true;
	ActorCollider->report = this;

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

	AnimationProgress += deltaTime;
	if (AnimationProgress > AnimationPeriod)
	{
		AnimationProgress -= AnimationPeriod;
		SetWanderMovement();
	}

	SetSpeed();

	Move(deltaTime, Direction);
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

void SpikeEnemy::TakeDamage()
{
}

void SpikeEnemy::OnContact(Physics::PhysicActor *physicActor)
{
	// resolved collision gives corrected position
	m_position = ActorCollider->pos;
	Player *player = (Player *)physicActor->report;
	player->TakeDamage();
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
