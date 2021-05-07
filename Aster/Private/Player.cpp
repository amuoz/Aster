#include "Player.h"

#include "Common.h"
#include "Sprite.h"
#include "Animation.h"

#include "SpriteRenderer.h"

const float DASH_PERIOD = 0.7;
const float DASH_SPEED_UP_START = DASH_PERIOD / 4;
const float DASH_SPEED_UP_FINISH = DASH_PERIOD * 3 / 4;
const float DASH_IFRAMES_START = DASH_PERIOD / 5;
const float DASH_IFRAMES_FINISH = DASH_PERIOD * 4 / 5;
const float BASE_SPEED = 200;

Player::Player(glm::vec3 pos, glm::vec3 size, Sprite *sprite, glm::vec3 color, glm::vec3 velocity) : Actor(pos, size, sprite, color, velocity)
{
	ActorCollider = g_PhysicsPtr->AddDynamicActor(pos, velocity, size, false, glm::vec3(0.0f), 1.0f);
	ActorCollider->bCheckCollision = true;
	ActorCollider->report = this;
	State = ActorState::IDLE;
	LastState = ActorState::IDLE;
	Inventory.push_back(PowerUpType::SPEAR);
	Speed = BASE_SPEED;
}

Player::~Player()
{
}

void Player::Render(Shader)
{
}

void Player::Update(float deltaTime, glm::vec4)
{
	if (State == ActorState::DASH)
	{
		DashTime += deltaTime;
		SetDashSpeed();
		SetDashIFrames();

		if (DashTime > DASH_PERIOD)
		{
			DashTime = 0;
			SetState(ActorState::IDLE);
		}
	}
	else
	{
		Speed = BASE_SPEED;
	}
}

void Player::Draw(SpriteRenderer &renderer, double deltatime)
{
	CurrentAnimation = GetAnimationFromState();

	m_sprite->Draw(CurrentAnimation, renderer, deltatime, m_position, m_scale, m_rotAngle, m_color);
}

AnimationType Player::GetAnimationFromState()
{
	switch (State)
	{
	case ActorState::IDLE:
	default:
		return AnimationType::IDLE;

	case ActorState::MOVEMENT_RIGHT:
	case ActorState::MOVEMENT_LEFT:
	case ActorState::MOVEMENT_DOWN:
	case ActorState::MOVEMENT_UP:
		return AnimationType::WALK;

	case ActorState::ATTACK_RIGHT:
		if (ActivePowerUp == PowerUpType::SWORD)
		{
			return AnimationType::SWORD_RIGHT;
		}
		else
		{
			return AnimationType::ATTACK_RIGHT;
		}
	case ActorState::ATTACK_LEFT:
		if (ActivePowerUp == PowerUpType::SWORD)
		{
			return AnimationType::SWORD_LEFT;
		}
		else
		{
			return AnimationType::ATTACK_LEFT;
		}
	case ActorState::ATTACK_DOWN:
		if (ActivePowerUp == PowerUpType::SWORD)
		{
			return AnimationType::SWORD_DOWN;
		}
		else
		{
			return AnimationType::ATTACK_DOWN;
		}
	case ActorState::ATTACK_UP:
		if (ActivePowerUp == PowerUpType::SWORD)
		{
			return AnimationType::SWORD_UP;
		}
		else
		{
			return AnimationType::ATTACK_UP;
		}

	case ActorState::DASH:
		bool isMovingRight = MovementDirection.x > 0 ||
												 (MovementDirection.x == 0 && LastMovementDirection.x >= 0);
		return isMovingRight ? AnimationType::DASH_RIGHT : AnimationType::DASH_LEFT;
	}
}

void Player::TakeDamage()
{
	if (State != ActorState::DASH || !IsInDashIFrames())
	{
		SetActive(false);
	}
}

bool Player::IsInDashIFrames()
{
	return (DashTime > DASH_IFRAMES_START) && (DashTime < DASH_IFRAMES_FINISH);
}

void Player::Move(float deltaTime, glm::vec3 direction)
{
	if (State == ActorState::DASH)
	{
		Actor::Move(deltaTime, LastMovementDirection);
	}
	else
	{
		if (direction.x > 0)
			SetState(ActorState::MOVEMENT_RIGHT);
		else if (direction.x < 0)
			SetState(ActorState::MOVEMENT_LEFT);
		else if (direction.y > 0)
			SetState(ActorState::MOVEMENT_DOWN);
		else if (direction.y < 0)
			SetState(ActorState::MOVEMENT_UP);
		else
			SetState(ActorState::IDLE);

		MovementDirection = direction;

		if (direction.x != 0 || direction.y != 0)
		{
			LastMovementDirection = direction;
		}

		Actor::Move(deltaTime, MovementDirection);
	}
}

void Player::Dash()
{
	DashTime = 0;
	SetState(ActorState::DASH);
}

void Player::SetDashSpeed()
{
	if (DashTime < DASH_SPEED_UP_START || DashTime > DASH_SPEED_UP_FINISH)
	{
		Speed = BASE_SPEED;
	}
	else
	{
		Speed = 2 * BASE_SPEED;
	}
}

void Player::SetDashIFrames()
{
	if (IsInDashIFrames())
	{
		ActorCollider->justReport = true;
	}
	else
	{
		ActorCollider->justReport = false;
	}
}

void Player::Idle()
{
	SetState(ActorState::IDLE);
}

void Player::Attack()
{
	if (State == ActorState::MOVEMENT_RIGHT)
		SetState(ActorState::ATTACK_RIGHT);
	else if (State == ActorState::MOVEMENT_LEFT)
		SetState(ActorState::ATTACK_LEFT);
	else if (State == ActorState::MOVEMENT_DOWN)
		SetState(ActorState::ATTACK_DOWN);
	else if (State == ActorState::MOVEMENT_UP)
		SetState(ActorState::ATTACK_UP);
	else if (LastState == ActorState::ATTACK_RIGHT)
		SetState(ActorState::ATTACK_RIGHT);
	else if (LastState == ActorState::ATTACK_LEFT)
		SetState(ActorState::ATTACK_LEFT);
	else if (LastState == ActorState::ATTACK_DOWN)
		SetState(ActorState::ATTACK_DOWN);
	else if (LastState == ActorState::ATTACK_UP)
		SetState(ActorState::ATTACK_UP);
	else if (State == ActorState::IDLE)
	{
		if (LastState == ActorState::MOVEMENT_RIGHT)
			SetState(ActorState::ATTACK_RIGHT);
		else if (LastState == ActorState::MOVEMENT_LEFT)
			SetState(ActorState::ATTACK_LEFT);
		else if (LastState == ActorState::MOVEMENT_DOWN)
			SetState(ActorState::ATTACK_DOWN);
		else if (LastState == ActorState::MOVEMENT_UP)
			SetState(ActorState::ATTACK_UP);
		else
			SetState(ActorState::ATTACK_RIGHT);
	}
	else
		SetState(ActorState::ATTACK_RIGHT);
}

void Player::OnContact(
		std::shared_ptr<Physics::PhysicActor>,
		std::shared_ptr<Physics::PhysicActor>)
{
	m_position = ActorCollider->pos;
}

bool Player::IsPlayer()
{
	return true;
}

glm::vec4 Player::GetAttackHitbox()
{
	glm::vec4 spriteHitbox = m_sprite->GetAttackHitbox(CurrentAnimation);

	if (spriteHitbox.x == 0 && spriteHitbox.y == 0 && spriteHitbox.z == 0 && spriteHitbox.w == 0)
	{
		return glm::vec4(0, 0, 0, 0);
	}

	return glm::vec4(
			m_position.x + spriteHitbox.x,
			m_position.y + spriteHitbox.y,
			spriteHitbox.z,
			spriteHitbox.w);
}

void Player::PowerUp(PowerUpType powerUp)
{
	Inventory.push_back(powerUp);
	ActivePowerUp = powerUp;
}

std::vector<PowerUpType> Player::GetPowerUps()
{
	return Inventory;
}

PowerUpType Player::GetActivePowerUp()
{
	return ActivePowerUp;
}

void Player::SelectPowerUp(unsigned int index)
{
	if (Inventory.size() > index)
	{
		ActivePowerUp = Inventory[index];
	}
}
