#include "Player.h"

#include "Sprite.h"
#include "Animation.h"

#include "SpriteRenderer.h"
#include "Physics.h"
#include "PhysicActor.h"

const float DASH_PERIOD = 0.7f;
const float DASH_SPEED_UP_START = DASH_PERIOD / 4;
const float DASH_SPEED_UP_FINISH = DASH_PERIOD * 3 / 4;
const float DASH_IFRAMES_START = DASH_PERIOD / 5;
const float DASH_IFRAMES_FINISH = DASH_PERIOD * 4 / 5;
const float BASE_SPEED = 200;
const float HAMMER_BLOCK_RATE = 0.5;

Player::Player(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color) : Actor(pos, size, std::move(sprite), color)
{
	ActorCollider = Physics::Get()->AddDynamicActor(pos, size, CollisionChannel::PLAYER);
	ActorCollider->bCheckCollision = true;
	ActorCollider->ChannelResponse[CollisionChannel::STATIC] = CollisionResponse::BLOCK;
	ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::BLOCK;
	ActorCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::IGNORED;

	State = ActorState::IDLE;
	LastState = ActorState::IDLE;
	Speed = BASE_SPEED;
	CurrentAnimation = AnimationType::IDLE;
	DashTime = 0.0f;
	ActivePowerUp = PowerUpType::NONE;
	InputDirection = glm::vec2(0);
	MovementDirection = glm::vec2(0);
	LastMovementDirection = glm::vec2(0);
	ZIndex = 0.6f;
}

Player::~Player()
{
	//std::cout << "Player destroyed" << std::endl;
}

void Player::Update(float deltaTime, glm::vec4 playerAttackHitbox)
{
	Actor::Update(deltaTime, playerAttackHitbox);

	AnimationProgress += deltaTime;

	if (IsDashState())
	{
		DashTime += deltaTime;
		SetDashSpeed();
		SetDashIFrames();

		if (DashTime > DASH_PERIOD)
		{
			DashTime = 0;
			SetState(ActorState::IDLE);
		}

		Actor::Move(deltaTime, LastMovementDirection);
	}
	else if (!IsBlockedByHammer())
	{
		Move(deltaTime, InputDirection);
	}
}

void Player::Draw(SpriteRenderer &renderer, double deltatime)
{
	if (IsActive())
	{
		CurrentAnimation = GetAnimationFromState();
		glm::vec3 spritePosition(Position, ZIndex);
		ActorSprite->Draw(CurrentAnimation, renderer, deltatime, spritePosition, m_scale, m_rotAngle, m_color);
	}
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
		else if (ActivePowerUp == PowerUpType::SPEAR)
		{
			return AnimationType::SPEAR_RIGHT;
		}
		else if (ActivePowerUp == PowerUpType::HAMMER)
		{
			return AnimationType::HAMMER_RIGHT;
		}
		else
		{
			return AnimationType::IDLE;
		}
	case ActorState::ATTACK_LEFT:
		if (ActivePowerUp == PowerUpType::SWORD)
		{
			return AnimationType::SWORD_LEFT;
		}
		else if (ActivePowerUp == PowerUpType::SPEAR)
		{
			return AnimationType::SPEAR_LEFT;
		}
		else if (ActivePowerUp == PowerUpType::HAMMER)
		{
			return AnimationType::HAMMER_LEFT;
		}
		else
		{
			return AnimationType::IDLE;
		}
	case ActorState::ATTACK_DOWN:
		if (ActivePowerUp == PowerUpType::SWORD)
		{
			return AnimationType::SWORD_DOWN;
		}
		else if (ActivePowerUp == PowerUpType::SPEAR)
		{
			return AnimationType::SPEAR_DOWN;
		}
		else if (ActivePowerUp == PowerUpType::HAMMER)
		{
			return AnimationType::HAMMER_RIGHT;
		}
		else
		{
			return AnimationType::IDLE;
		}
	case ActorState::ATTACK_UP:
		if (ActivePowerUp == PowerUpType::SWORD)
		{
			return AnimationType::SWORD_UP;
		}
		else if (ActivePowerUp == PowerUpType::SPEAR)
		{
			return AnimationType::SPEAR_UP;
		}
		else if (ActivePowerUp == PowerUpType::HAMMER)
		{
			return AnimationType::HAMMER_RIGHT;
		}
		else
		{
			return AnimationType::IDLE;
		}

	case ActorState::DASH_RIGHT:
		return AnimationType::DASH_RIGHT;
	case ActorState::DASH_LEFT:
		return AnimationType::DASH_LEFT;
	case ActorState::DASH_DOWN:
	case ActorState::DASH_UP:
		bool isMovingRight = MovementDirection.x > 0 ||
												 (MovementDirection.x == 0 && LastMovementDirection.x >= 0);
		return isMovingRight ? AnimationType::DASH_RIGHT : AnimationType::DASH_LEFT;
	}
}

bool Player::IsAttackAnimationPlaying()
{
	float animationLength = ActorSprite->GetAnimationLength();
	bool isAnimationPlaying = animationLength && AnimationProgress < animationLength;
	bool isTryingToChangeStateFromAttack = State == ActorState::ATTACK_UP ||
																				 State == ActorState::ATTACK_RIGHT ||
																				 State == ActorState::ATTACK_DOWN ||
																				 State == ActorState::ATTACK_LEFT;

	return isAnimationPlaying && isTryingToChangeStateFromAttack;
}

void Player::TakeDamage()
{
	if (!IsDashState() || !IsInDashIFrames())
	{
		SetActive(false);
	}
}

bool Player::IsInDashIFrames()
{
	return (DashTime > DASH_IFRAMES_START) && (DashTime < DASH_IFRAMES_FINISH);
}

void Player::SetInputDirection(glm::vec2 direction)
{
	InputDirection = direction;
}

void Player::Move(float deltaTime, glm::vec2 direction)
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

void Player::Dash(glm::vec2 direction)
{
	if (IsDashState())
	{
		return;
	}

	if (direction.x > 0 || LastState == ActorState::DASH_RIGHT)
		SetState(ActorState::DASH_RIGHT);
	else if (direction.x < 0 || LastState == ActorState::DASH_LEFT)
		SetState(ActorState::DASH_LEFT);
	else if (direction.y > 0 || LastState == ActorState::DASH_DOWN)
		SetState(ActorState::DASH_DOWN);
	else if (direction.y < 0 || LastState == ActorState::DASH_UP)
		SetState(ActorState::DASH_UP);
	else
		SetState(ActorState::IDLE);
}

bool Player::IsDashState()
{
	return State == ActorState::DASH_UP ||
				 State == ActorState::DASH_RIGHT ||
				 State == ActorState::DASH_DOWN ||
				 State == ActorState::DASH_LEFT;
}

bool Player::IsBlockedByHammer()
{
	float animationLength = ActorSprite->GetAnimationLength();
	bool isInHammerBlockingFrames = AnimationProgress < animationLength * HAMMER_BLOCK_RATE;

	return IsHammerAttack() && isInHammerBlockingFrames;
}

bool Player::IsHammerAttack()
{
	return (CurrentAnimation == AnimationType::HAMMER_RIGHT) || (CurrentAnimation == AnimationType::HAMMER_LEFT);
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
		//ActorCollider->justReport = true;
		ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::IGNORED;
	}
	else
	{
		//ActorCollider->justReport = false;
		ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::BLOCK;
	}
}

void Player::Idle()
{
	SetState(ActorState::IDLE);
}

void Player::Attack()
{
	if (IsAttackPossible())
	{
		if (State == ActorState::MOVEMENT_RIGHT ||
				State == ActorState::DASH_RIGHT)
			SetState(ActorState::ATTACK_RIGHT);
		else if (State == ActorState::MOVEMENT_LEFT ||
						 State == ActorState::DASH_LEFT)
			SetState(ActorState::ATTACK_LEFT);
		else if (State == ActorState::MOVEMENT_DOWN ||
						 State == ActorState::DASH_DOWN)
			SetState(ActorState::ATTACK_DOWN);
		else if (State == ActorState::MOVEMENT_UP ||
						 State == ActorState::DASH_UP)
			SetState(ActorState::ATTACK_UP);
		else if (State == ActorState::IDLE)
		{
			if (LastState == ActorState::ATTACK_RIGHT ||
					LastState == ActorState::MOVEMENT_RIGHT ||
					LastState == ActorState::DASH_RIGHT)
				SetState(ActorState::ATTACK_RIGHT);
			else if (LastState == ActorState::ATTACK_LEFT ||
							 LastState == ActorState::MOVEMENT_LEFT ||
							 LastState == ActorState::DASH_LEFT)
				SetState(ActorState::ATTACK_LEFT);
			else if (LastState == ActorState::ATTACK_DOWN ||
							 LastState == ActorState::MOVEMENT_DOWN ||
							 LastState == ActorState::DASH_DOWN)
				SetState(ActorState::ATTACK_DOWN);
			else if (LastState == ActorState::ATTACK_UP ||
							 LastState == ActorState::MOVEMENT_UP ||
							 LastState == ActorState::DASH_UP)
				SetState(ActorState::ATTACK_UP);
			else
				SetState(ActorState::ATTACK_RIGHT);
		}
		else
			SetState(ActorState::ATTACK_RIGHT);
	}
}

bool Player::IsAttackPossible()
{
	return (ActivePowerUp != PowerUpType::NONE) &&
				 !IsDashState();
}

bool Player::IsPlayer()
{
	return true;
}

void Player::SetState(ActorState state)
{
	if (!IsAttackAnimationPlaying())
	{
		Actor::SetState(state);
	}
}

glm::vec4 Player::GetAttackHitbox()
{
	glm::vec4 spriteHitbox = ActorSprite->GetAttackHitbox(CurrentAnimation);

	if (spriteHitbox.x == 0 && spriteHitbox.y == 0 && spriteHitbox.z == 0 && spriteHitbox.w == 0)
	{
		return glm::vec4(0, 0, 0, 0);
	}

	return glm::vec4(
			Position.x + spriteHitbox.x,
			Position.y + spriteHitbox.y,
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
