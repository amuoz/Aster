#include "SpikeEnemy.h"

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Sprite.h"
#include "Player.h"
#include "Physics.h"
#include "SpriteRenderer.h"
#include "PhysicActor.h"

const float STILL_CHANCE = 0.3;
const float CHANGE_DIRECTION_CHANCE = 0.2;
const float AGGRO_SIZE = 350;

SpikeEnemy::SpikeEnemy(glm::vec3 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, float framePeriod, glm::vec3 color) : Actor(pos, size, std::move(sprite), color)
{
	ActorCollider = Physics::Get()->AddDynamicActor(pos, size, CollisionChannel::DYNAMIC);
	ActorCollider->bCheckCollision = true;
	ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::BLOCK;
	ActorCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::OVERLAP;

	AggroCollider = Physics::Get()->AddDynamicActor(
			GetAggroPosition(pos, size),
			glm::vec3(AGGRO_SIZE, AGGRO_SIZE, 0),
			CollisionChannel::DYNAMIC);
	AggroCollider->bCheckCollision = true;
	AggroCollider->ChannelResponse[CollisionChannel::STATIC] = CollisionResponse::IGNORED;
	AggroCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::IGNORED;
	AggroCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::OVERLAP;
	// C++ 11 lambda for setting class function to a function ptr
	AggroCollider->OnBeginOverlapPtr = [=](std::shared_ptr<PhysicActor> other) {
		this->OnBeginOverlapAggro(other);
	};
	AggroCollider->OnEndOverlapPtr = [=](std::shared_ptr<PhysicActor> other) {
		this->OnEndOverlapAggro(other);
	};

	int animationFrames = ActorSprite->GetFramesCount();
	AnimationPeriod = framePeriod * animationFrames;
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
	//std::cout << "SpikeEnemy destroyed" << std::endl;
}

void SpikeEnemy::BeginPlay()
{
	Actor::BeginPlay();

	AggroCollider->report = shared_from_this();
}

void SpikeEnemy::Update(float deltaTime, glm::vec4 attackHitbox)
{
	Actor::Update(deltaTime, attackHitbox);

	if (IsAttacked(attackHitbox))
	{
		IsDestroyed = true;
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

	ActorSprite->Draw(
			CurrentAnimation,
			renderer,
			deltatime,
			m_position,
			m_scale,
			m_rotAngle,
			m_color);
}

void SpikeEnemy::Destroy()
{
	Physics::Get()->DeleteDynamicActor(AggroCollider);

	Actor::Destroy();
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
	// 30% still
	if (PassRandomChance(STILL_CHANCE))
	{
		Direction = glm::vec3(0, 0, 0);
	}
	// 70% moving
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
	Direction = glm::normalize(AggroedActor->GetPosition() - m_position);
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

void SpikeEnemy::OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	other->report->TakeDamage();
}

void SpikeEnemy::OnEndOverlapFunction(std::shared_ptr<PhysicActor> other)
{

}

void SpikeEnemy::OnBeginOverlapAggro(std::shared_ptr<PhysicActor> other)
{
	if (other->report->IsPlayer())
	{
		SetState(ActorState::AGGRO);
		AggroedActor = other->report;
	}
}

void SpikeEnemy::OnEndOverlapAggro(std::shared_ptr<PhysicActor> other)
{
	if (other->report->IsPlayer())
	{
		SetState(ActorState::IDLE);
		AggroedActor.reset();
	}
}
