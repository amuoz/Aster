#include "Player.h"

#include "Common.h"
#include "Sprite.h"
#include "Animation.h"

Player::Player(glm::vec3 pos, glm::vec3 size, Sprite *sprite, glm::vec3 color, glm::vec3 velocity) : Actor(pos, size, sprite, color, velocity)
{
	m_physicsActor = g_PhysicsPtr->AddDynamicActor(pos, velocity, size, glm::vec3(0.0f), 1.0f);
	m_physicsActor->bCheckCollision = true;
	m_physicsActor->report = this;
	State = PlayerState::IDLE;
	LastState = PlayerState::IDLE;
}

Player::~Player()
{
}

void Player::Render(Shader)
{
}

void Player::Update(float, glm::vec4)
{
}

void Player::Draw(SpriteRenderer &renderer, double deltatime)
{
	switch (State)
	{
	case PlayerState::IDLE:
		CurrentAnimation = AnimationType::IDLE;
		break;
	case PlayerState::MOVEMENT_RIGHT:
	case PlayerState::MOVEMENT_LEFT:
	case PlayerState::MOVEMENT_DOWN:
	case PlayerState::MOVEMENT_UP:
		CurrentAnimation = AnimationType::WALK;
		break;
	case PlayerState::ATTACK_RIGHT:
		CurrentAnimation = AnimationType::ATTACK_RIGHT;
		break;
	case PlayerState::ATTACK_LEFT:
		CurrentAnimation = AnimationType::ATTACK_LEFT;
		break;
	case PlayerState::ATTACK_DOWN:
		CurrentAnimation = AnimationType::ATTACK_DOWN;
		break;
	case PlayerState::ATTACK_UP:
		CurrentAnimation = AnimationType::ATTACK_UP;
		break;

	default:
		break;
	}

	m_sprite->Draw(CurrentAnimation, renderer, deltatime, m_position, m_scale, m_rotAngle, m_color);
}

void Player::Move(float deltaTime, glm::vec3 direction)
{
	if (direction.x > 0)
		SetState(PlayerState::MOVEMENT_RIGHT);
	else if (direction.x < 0)
		SetState(PlayerState::MOVEMENT_LEFT);
	else if (direction.y > 0)
		SetState(PlayerState::MOVEMENT_DOWN);
	else if (direction.y < 0)
		SetState(PlayerState::MOVEMENT_UP);
	else
		SetState(PlayerState::IDLE);

	float velocity = PLAYER_VELOCITY * deltaTime;
	SetPosition(velocity * direction);
}

void Player::Idle()
{
	SetState(PlayerState::IDLE);
}

void Player::Attack()
{
	if (State == PlayerState::MOVEMENT_RIGHT)
		SetState(PlayerState::ATTACK_RIGHT);
	else if (State == PlayerState::MOVEMENT_LEFT)
		SetState(PlayerState::ATTACK_LEFT);
	else if (State == PlayerState::MOVEMENT_DOWN)
		SetState(PlayerState::ATTACK_DOWN);
	else if (State == PlayerState::MOVEMENT_UP)
		SetState(PlayerState::ATTACK_UP);
	else if (LastState == PlayerState::ATTACK_RIGHT)
		SetState(PlayerState::ATTACK_RIGHT);
	else if (LastState == PlayerState::ATTACK_LEFT)
		SetState(PlayerState::ATTACK_LEFT);
	else if (LastState == PlayerState::ATTACK_DOWN)
		SetState(PlayerState::ATTACK_DOWN);
	else if (LastState == PlayerState::ATTACK_UP)
		SetState(PlayerState::ATTACK_UP);
	else if (State == PlayerState::IDLE)
	{
		if (LastState == PlayerState::MOVEMENT_RIGHT)
			SetState(PlayerState::ATTACK_RIGHT);
		else if (LastState == PlayerState::MOVEMENT_LEFT)
			SetState(PlayerState::ATTACK_LEFT);
		else if (LastState == PlayerState::MOVEMENT_DOWN)
			SetState(PlayerState::ATTACK_DOWN);
		else if (LastState == PlayerState::MOVEMENT_UP)
			SetState(PlayerState::ATTACK_UP);
		else
			SetState(PlayerState::ATTACK_RIGHT);
	}
	else
		SetState(PlayerState::ATTACK_RIGHT);
}

void Player::SetState(PlayerState state)
{
	if (State != state)
	{
		LastState = State;
	}

	State = state;
}

void Player::OnContact(Physics::PhysicActor *physicActor)
{
	// resolved collision gives corrected position
	m_position = m_physicsActor->pos;
}

glm::vec4 Player::GetAttackHitbox()
{
	glm::vec4 spriteHitbox = m_sprite->GetAttackHitbox(CurrentAnimation);

	return glm::vec4(
		m_position.x - m_scale.x + spriteHitbox.x,
		m_position.y - m_scale.y + spriteHitbox.y,
		spriteHitbox.z,
		spriteHitbox.w
	);
}
