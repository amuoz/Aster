#include "ShootyEnemy.h"

#include <cstdlib>
#include <time.h>
#include <cmath> // std::abs
#include <iostream>
#include <glm/glm.hpp>

#include "Common.h"
#include "Config.h"
#include "Sprite.h"
#include "Entity/Player.h"
#include "Physics.h"
#include "SpriteRenderer.h"
#include "PhysicActor.h"

ShootyEnemy::ShootyEnemy(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, float framePeriod, glm::vec4 color)
		: Enemy(pos, size, std::move(sprite), framePeriod, color)
{
	StillChance = Config::Get()->GetValue(SHOOTY_STILL_CHANCE);
	ChangeDirectionChance = Config::Get()->GetValue(SHOOTY_CHANGE_DIRECTION_CHANCE);
	AggroSize = Config::Get()->GetValue(SHOOTY_AGGRO_SIZE);

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
	AggroCollider->OnBeginOverlapPtr = [=](std::shared_ptr<PhysicActor> other)
	{
		this->OnBeginOverlapAggro(other);
	};
	AggroCollider->OnEndOverlapPtr = [=](std::shared_ptr<PhysicActor> other)
	{
		this->OnEndOverlapAggro(other);
	};

	srand(time(NULL));
}

ShootyEnemy::~ShootyEnemy()
{
	//std::cout << "ShootyEnemy destroyed" << std::endl;
}

void ShootyEnemy::BeginPlay()
{
	Enemy::BeginPlay();

	AggroCollider->report = shared_from_this();
}

void ShootyEnemy::Update(float deltaTime, glm::vec4 attackHitbox)
{
	Enemy::Update(deltaTime, attackHitbox);
}

void ShootyEnemy::Draw(SpriteRenderer &renderer, double deltatime)
{
	Enemy::Draw(renderer, deltatime);
}

AnimationType ShootyEnemy::GetAnimationFromState()
{
	if (Direction.y < 0)
	{
		if (std::abs(Direction.y) > std::abs(Direction.x))
			return AnimationType::IDLE_UP;
		else if (Direction.x > 0)
			return AnimationType::IDLE_RIGHT;
		else
			return AnimationType::IDLE_LEFT;
	}
	else
	{
		if (std::abs(Direction.y) > std::abs(Direction.x))
			return AnimationType::IDLE_DOWN;
		else if (Direction.x > 0)
			return AnimationType::IDLE_RIGHT;
		else
			return AnimationType::IDLE_LEFT;
	}
}

void ShootyEnemy::SetSpeed()
{
	Speed = MAX_SPEED;
}

void ShootyEnemy::OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	other->report->TakeDamage();
}

void ShootyEnemy::OnEndOverlapFunction(std::shared_ptr<PhysicActor> other)
{
}

void ShootyEnemy::OnBeginOverlapAggro(std::shared_ptr<PhysicActor> other)
{
	if (other->report->IsPlayer())
	{
		SetState(ActorState::AGGRO);
		AggroedActor = other->report;
	}
}

void ShootyEnemy::OnEndOverlapAggro(std::shared_ptr<PhysicActor> other)
{
	if (other->report->IsPlayer())
	{
		SetState(ActorState::IDLE);
		AggroedActor.reset();
	}
}
