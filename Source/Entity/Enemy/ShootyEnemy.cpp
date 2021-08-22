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
#include "Entity/Projectile.h"
#include "Physics.h"
#include "SpriteRenderer.h"
#include "PhysicActor.h"

ShootyEnemy::ShootyEnemy(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, float framePeriod, glm::vec4 color)
		: Enemy(pos, size, std::move(sprite), framePeriod, color)
{

	// std::cout << "Enemy pos: " << pos.x << ", " << pos.y << std::endl;
	// std::cout << "Enemy size: " << size.x << ", " << size.y << std::endl;

	StillChance = Config::Get()->GetValue(SHOOTY_STILL_CHANCE);
	ChangeDirectionChance = Config::Get()->GetValue(SHOOTY_CHANGE_DIRECTION_CHANCE);
	AggroSize = Config::Get()->GetValue(SHOOTY_AGGRO_SIZE);
	ShootDelay = Config::Get()->GetValue(SHOOTY_SHOOT_DELAY);
	ShootInterval = Config::Get()->GetValue(SHOOTY_SHOOT_INTERVAL);
	AggroProgress = 0;
	AttackProgress = 0;
	hasShot = false;

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

	switch (State)
	{
	case ActorState::IDLE:
	default:
		break;
	case ActorState::AGGRO:
		UpdateAggroState(deltaTime, attackHitbox);
		break;
	case ActorState::SHOOT:
		UpdateShootState(deltaTime, attackHitbox);
		break;
	case ActorState::ATTACK_UP:
	case ActorState::ATTACK_RIGHT:
	case ActorState::ATTACK_DOWN:
	case ActorState::ATTACK_LEFT:
		UpdateAttackState(deltaTime, attackHitbox);
		break;
	}

	UpdateArrow(deltaTime, attackHitbox);
}

void ShootyEnemy::UpdateAggroState(float deltaTime, glm::vec4 attackHitbox)
{
	AggroProgress += deltaTime;

	if (AggroProgress > ShootInterval)
	{
		std::cout << "Shooty SHOOT" << std::endl;
		SetState(ActorState::SHOOT);
		AggroProgress = 0;
	}
}

void ShootyEnemy::UpdateShootState(float deltaTime, glm::vec4 attackHitbox)
{
	AttackProgress += deltaTime;

	if (AttackProgress > ShootDelay)
	{
		std::cout << "Shooty ATTACK" << std::endl;
		SetAttackState();
		AttackProgress = 0;
	}
}

void ShootyEnemy::UpdateAttackState(float deltaTime, glm::vec4 attackHitbox)
{
	if (!hasShot)
	{
		CreateArrow(State);
		hasShot = true;
	}
}

void ShootyEnemy::UpdateArrow(float deltaTime, glm::vec4 attackHitbox)
{
	if (Arrow)
	{
		if (Arrow->IsDestroyed)
		{
			Arrow->Destroy();
		}
		else
		{
			Arrow->Update(deltaTime, attackHitbox);
		}
	}
}

void ShootyEnemy::OnAnimationEnd()
{
	if (IsAttackState())
	{
		hasShot = false;
		std::cout << "Shooty AGGRO" << std::endl;
		SetState(ActorState::AGGRO);
	}
}

void ShootyEnemy::SetAttackState()
{
	auto objectiveDirection = GetAggroDirection();

	if (objectiveDirection.y < 0)
	{
		if (std::abs(objectiveDirection.y) > std::abs(objectiveDirection.x))
			SetState(ActorState::ATTACK_UP);
		else if (objectiveDirection.x > 0)
			SetState(ActorState::ATTACK_RIGHT);
		else
			SetState(ActorState::ATTACK_LEFT);
	}
	else
	{
		if (std::abs(objectiveDirection.y) > std::abs(objectiveDirection.x))
			SetState(ActorState::ATTACK_DOWN);
		else if (objectiveDirection.x > 0)
			SetState(ActorState::ATTACK_RIGHT);
		else
			SetState(ActorState::ATTACK_LEFT);
	}
}

void ShootyEnemy::CreateArrow(ActorState attackDirection)
{
	const glm::vec3 ARROW_SIZE(16.0f, 9.0f, 0.0f);
	glm::vec3 scale(1.0f, 1.0f, 1.0f);
	scale.x = Config::Get()->GetValue(SRC_WIDTH) / ARROW_SIZE.x;
	scale.y = Config::Get()->GetValue(SRC_HEIGHT) / ARROW_SIZE.y;

	std::string spriteName;
	glm::vec2 arrowDirection;
	switch (attackDirection)
	{
	case ActorState::ATTACK_UP:
	default:
		spriteName = "shooty_arrow_up";
		arrowDirection = glm::vec2(0, -1);
		break;
	case ActorState::ATTACK_RIGHT:
		spriteName = "shooty_arrow_right";
		arrowDirection = glm::vec2(1, 0);
		break;
	case ActorState::ATTACK_DOWN:
		spriteName = "shooty_arrow_down";
		arrowDirection = glm::vec2(0, 1);
		break;
	case ActorState::ATTACK_LEFT:
		spriteName = "shooty_arrow_left";
		arrowDirection = glm::vec2(-1, 0);
		break;
	}

	auto sprite = std::make_unique<Sprite>(spriteName);
	float framePeriod = 0.10f;
	sprite->AddAnimation("shooty_arrow_fire", AnimationType::IDLE, framePeriod);

	Arrow = std::make_unique<Projectile>(
			GetArrowPosition(),
			scale,
			arrowDirection,
			std::move(sprite));
}

glm::vec2 ShootyEnemy::GetArrowPosition()
{
	return glm::vec2(
			Position.x,
			Position.y - Config::Get()->GetValue(CELL_HEIGHT));
}

void ShootyEnemy::Draw(SpriteRenderer &renderer, double deltatime)
{
	Enemy::Draw(renderer, deltatime);

	DrawArrow(renderer, deltatime);
}

void ShootyEnemy::DrawArrow(SpriteRenderer &renderer, double deltatime)
{
	if (Arrow && !Arrow->IsDestroyed)
	{
		Arrow->Draw(renderer, deltatime);
	}
}

AnimationType ShootyEnemy::GetAnimationFromState()
{
	switch (State)
	{
	case ActorState::IDLE:
	case ActorState::AGGRO:
	default:
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
	case ActorState::SHOOT:
	case ActorState::ATTACK_UP:
	case ActorState::ATTACK_RIGHT:
	case ActorState::ATTACK_DOWN:
	case ActorState::ATTACK_LEFT:
		return GetShootAnimation();
	}
}

AnimationType ShootyEnemy::GetShootAnimation()
{
	auto objectiveDirection = GetAggroDirection();

	if (objectiveDirection.y < 0)
	{
		if (std::abs(objectiveDirection.y) > std::abs(objectiveDirection.x))
			return AnimationType::WALK_UP;
		else if (objectiveDirection.x > 0)
			return AnimationType::WALK_RIGHT;
		else
			return AnimationType::WALK_LEFT;
	}
	else
	{
		if (std::abs(objectiveDirection.y) > std::abs(objectiveDirection.x))
			return AnimationType::WALK_DOWN;
		else if (objectiveDirection.x > 0)
			return AnimationType::WALK_RIGHT;
		else
			return AnimationType::WALK_LEFT;
	}
}

void ShootyEnemy::SetSpeed()
{
	switch (State)
	{
	case ActorState::IDLE:
	default:
		Speed = MAX_SPEED;
		break;
	case ActorState::AGGRO:
		Speed = MAX_SPEED / 5;
		break;
	case ActorState::SHOOT:
	case ActorState::ATTACK_UP:
	case ActorState::ATTACK_RIGHT:
	case ActorState::ATTACK_DOWN:
	case ActorState::ATTACK_LEFT:
		Speed = 0.0f;
		break;
	}
}

glm::vec2 ShootyEnemy::GetAggroDirection()
{
	return glm::normalize(AggroedActor->GetPosition() - Position);
}

void ShootyEnemy::OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	// other->report->TakeDamage();
}

void ShootyEnemy::OnEndOverlapFunction(std::shared_ptr<PhysicActor> other)
{
}

void ShootyEnemy::OnBeginOverlapAggro(std::shared_ptr<PhysicActor> other)
{
	if (other->report->IsPlayer() && State == ActorState::IDLE)
	{
		std::cout << "Shooty AGGRO" << std::endl;
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
