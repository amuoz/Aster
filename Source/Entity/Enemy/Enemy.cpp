#include "Enemy.h"

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Common.h"
#include "Config.h"
#include "Sprite.h"
#include "Entity/Player.h"
#include "Physics.h"
#include "SpriteRenderer.h"
#include "PhysicActor.h"

Enemy::Enemy(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, float framePeriod, glm::vec4 color)
		: Actor(pos, size, std::move(sprite), color)
{
	int animationFrames = ActorSprite->GetFramesCount();
	AnimationPeriod = framePeriod * animationFrames;
	AnimationProgress = 0.0f;
	State = ActorState::IDLE;
	LastState = ActorState::IDLE;
	Direction = glm::vec2(0, 0);
	Speed = MAX_SPEED;
	CurrentAnimation = AnimationType::IDLE;
	ChangeDirectionPeriod = 1.0f;
	LastDirectionChange = 0.0f;
	ZIndex = 0.7f;
	srand(time(NULL));
}

Enemy::~Enemy()
{
	//std::cout << "Enemy destroyed" << std::endl;
}

void Enemy::BeginPlay()
{
	Actor::BeginPlay();
}

void Enemy::Update(float deltaTime, glm::vec4 attackHitbox)
{
	Actor::Update(deltaTime, attackHitbox);

	if (IsAttacked(attackHitbox))
	{
		IsDestroyed = true;
	}

	int animationFrames = ActorSprite->GetFramesCount();
	float framePeriod = ActorSprite->GetAnimationSpeed();
	AnimationPeriod = framePeriod * animationFrames;
	AnimationProgress += deltaTime;

	if (AnimationProgress > AnimationPeriod)
	{
		AnimationProgress -= AnimationPeriod;

		switch (State)
		{
		case ActorState::IDLE:
		default:
			Direction = SetWanderMovement();
			break;
		case ActorState::AGGRO:
			Direction = SetAggroMovement();
			break;
		}
	}

	SetSpeed();

	Move(deltaTime, Direction);

	AggroCollider->pos = GetAggroPosition(Position, m_scale);
}

void Enemy::Draw(SpriteRenderer &renderer, double deltatime)
{
	Color = GetColorFromState();
	CurrentAnimation = GetAnimationFromState();
	glm::vec3 spritePosition(Position, ZIndex);
	ActorSprite->Draw(
			CurrentAnimation,
			renderer,
			deltatime,
			spritePosition,
			m_scale,
			m_rotAngle,
			Color);
}

glm::vec4 Enemy::GetColorFromState()
{
	switch (State)
	{
	case ActorState::IDLE:
		return glm::vec4(1, 1, 1, 1);
	case ActorState::AGGRO:
		return glm::vec4(1, 0.5, 0.5, 1);
	default:
		break;
	}
}

AnimationType Enemy::GetAnimationFromState()
{
	return AnimationType::IDLE;
}

void Enemy::Destroy()
{
	Physics::Get()->DeleteDynamicActor(AggroCollider);

	Actor::Destroy();
}

bool Enemy::PassRandomChance(float chance)
{
	return (float(rand()) / float(RAND_MAX)) < chance;
}

float Enemy::GetRandomDirectionComponent()
{
	return (float(rand()) / float(RAND_MAX)) * 2 - 1;
}

glm::vec2 Enemy::GetRandomDirection()
{
	return glm::vec2(
			GetRandomDirectionComponent(),
			GetRandomDirectionComponent());
}

glm::vec2 Enemy::SetWanderMovement()
{
	// 30% still
	if (PassRandomChance(StillChance))
	{
		return glm::vec2(0, 0);
	}
	// 70% moving
	else
	{
		// 20% change direction
		if (PassRandomChance(ChangeDirectionChance))
		{
			return GetRandomDirection();
		}
		// 80% maintain direction
		else
		{
			return Direction;
		}
	}
}

glm::vec2 Enemy::SetAggroMovement()
{
	return glm::normalize(AggroedActor->GetPosition() - Position);
}

glm::vec2 Enemy::GetAggroPosition(glm::vec2 actorPosition, glm::vec3 actorSize)
{
	float posCorrectionX = (AggroSize - actorSize.x) / 2;
	float posCorrectionY = (AggroSize - actorSize.y) / 2;

	return glm::vec2(
			actorPosition.x - posCorrectionX,
			actorPosition.y + posCorrectionY);
}
