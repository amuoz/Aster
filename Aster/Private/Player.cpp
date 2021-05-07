#include "Player.h"

#include "Common.h"
#include "Sprite.h"
#include "Animation.h"

#include "SpriteRenderer.h"

Player::Player(glm::vec3 pos, glm::vec3 size, Sprite *sprite, glm::vec3 color, glm::vec3 velocity) : Actor(pos, size, sprite, color, velocity)
{
	m_physicsActor = g_PhysicsPtr->AddDynamicActor(pos, velocity, size, glm::vec3(0.0f), 1.0f);
	m_physicsActor->bCheckCollision = true;
	m_physicsActor->report = this;
	State = ActorState::IDLE;
	LastState = ActorState::IDLE;
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
	case ActorState::IDLE:
		CurrentAnimation = AnimationType::IDLE;
		break;
	case ActorState::MOVEMENT_RIGHT:
	case ActorState::MOVEMENT_LEFT:
	case ActorState::MOVEMENT_DOWN:
	case ActorState::MOVEMENT_UP:
		CurrentAnimation = AnimationType::WALK;
		break;
	case ActorState::ATTACK_RIGHT:
		CurrentAnimation = AnimationType::ATTACK_RIGHT;
		break;
	case ActorState::ATTACK_LEFT:
		CurrentAnimation = AnimationType::ATTACK_LEFT;
		break;
	case ActorState::ATTACK_DOWN:
		CurrentAnimation = AnimationType::ATTACK_DOWN;
		break;
	case ActorState::ATTACK_UP:
		CurrentAnimation = AnimationType::ATTACK_UP;
		break;

	default:
		break;
	}

	m_sprite->Draw(CurrentAnimation, renderer, deltatime, m_position, m_scale, m_rotAngle, m_color);
}

void Player::TakeDamage()
{
	SetActive(false);
}

void Player::Move(float deltaTime, glm::vec3 direction)
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

	float velocity = PLAYER_VELOCITY * deltaTime;
	SetPosition(velocity * direction);
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

void Player::SetState(ActorState state)
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

	if (spriteHitbox.x == 0 && spriteHitbox.y == 0
		&& spriteHitbox.z == 0 && spriteHitbox.w == 0)
	{
		return glm::vec4(0, 0, 0, 0);
	}

	return glm::vec4(
		m_position.x + spriteHitbox.x,
		m_position.y + spriteHitbox.y,
		spriteHitbox.z,
		spriteHitbox.w
	);
}
