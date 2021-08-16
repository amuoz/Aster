#include "SpikeEnemy.h"

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

SpikeEnemy::SpikeEnemy(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, float framePeriod, glm::vec4 color)
		: Enemy(pos, size, std::move(sprite), framePeriod, color)
{
	StillChance = Config::Get()->GetValue(SPIKE_STILL_CHANCE);
	ChangeDirectionChance = Config::Get()->GetValue(SPIKE_CHANGE_DIRECTION_CHANCE);
	AggroSize = Config::Get()->GetValue(SPIKE_AGGRO_SIZE);

	ActorCollider = Physics::Get()->AddDynamicActor(pos, size, CollisionChannel::DYNAMIC);
	ActorCollider->bCheckCollision = true;
	ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::BLOCK;
	ActorCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::OVERLAP;

	AggroCollider = Physics::Get()->AddDynamicActor(
			GetAggroPosition(pos, size),
			glm::vec3(AggroSize, AggroSize, 0),
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

	srand(time(NULL));
}

SpikeEnemy::~SpikeEnemy()
{
	//std::cout << "SpikeEnemy destroyed" << std::endl;
}

void SpikeEnemy::BeginPlay()
{
	Enemy::BeginPlay();

	AggroCollider->report = shared_from_this();
}

void SpikeEnemy::Update(float deltaTime, glm::vec4 attackHitbox)
{
	Enemy::Update(deltaTime, attackHitbox);
}

void SpikeEnemy::Draw(SpriteRenderer &renderer, double deltatime)
{
	Enemy::Draw(renderer, deltatime);
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
