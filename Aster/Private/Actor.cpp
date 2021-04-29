#include "Actor.h"

#include "Common.h"
#include "SpriteRenderer.h"
#include "Sprite.h"

Actor::Actor()
{
	m_position = glm::vec3(0.0f);
	m_scale = glm::vec3(1.0f);
	m_velocity = glm::vec3(0.0f);
	m_color = glm::vec3(1.0f);
	m_rotAngle = 0.0f;
	m_radius = 1.0f;
	m_rotAxis = glm::vec3(0.0f);

	m_active = true;
	m_delete = false;
	IsDestroyable = false;
	IsDestroyed = false;
}

Actor::Actor(glm::vec3 pos, glm::vec3 size, Sprite* sprite, glm::vec3 color, glm::vec3 velocity)
{
	m_position = pos;
	m_scale = size;
	m_velocity = velocity;
	m_color = color;
	m_sprite = sprite;

	m_rotAngle = 0.0f;
	m_radius = size.x / 2;
	m_rotAxis = glm::vec3(0.0f);

	m_active = true;
	m_delete = false;
	IsDestroyable = false;
	IsDestroyed = false;
}

Actor::~Actor()
{
	g_PhysicsPtr->DeleteDynamicActor(m_physicsActor);
	delete m_physicsActor;
	delete m_mesh;
}

void Actor::Reset()
{

}

void Actor::Draw(SpriteRenderer& renderer, double)
{
	renderer.DrawSprite(m_sprite, m_position, m_scale, m_rotAngle, m_color);
}

void Actor::Move(float deltaTime, glm::vec3 direction)
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

	float velocity = VELOCITY * deltaTime;
	SetPosition(velocity * direction);
}

void Actor::SetState(ActorState state)
{
	if (State != state)
	{
		LastState = State;
	}

	State = state;
}

void Actor::SetActive(bool newActive)
{
	m_physicsActor->active = newActive;
	m_active = newActive;
}

void Actor::SetDelete(bool newDelete)
{
	m_delete = newDelete;
}

void Actor::SetColor(glm::vec3 color)
{
	m_color = color;
}

void Actor::SetPosition(glm::vec3 pos)
{
	if (!m_physicsActor->bSimulate)
	{
		m_position += pos;
		// update physics position. Not simulating physics
		m_physicsActor->pos += pos;
	}
}

bool Actor::IsAttacked(glm::vec4 attackHitbox)
{
	float xHitbox = attackHitbox.x;
	float yHitbox = attackHitbox.y;
	float widthHitbox = attackHitbox.z;
	float heightHitbox = attackHitbox.w;

	float blockWidth = m_scale.x;
	float blockHeight = m_scale.y;

	bool xCollision = m_position.x + blockWidth >= xHitbox && xHitbox + widthHitbox >= m_position.x;
	
	bool yCollision = m_position.y + blockHeight >= yHitbox && yHitbox + heightHitbox >= m_position.y;

	return xCollision && yCollision;
}
