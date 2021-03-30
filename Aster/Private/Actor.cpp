#include "Actor.h"

#include "Common.h"
#include "SpriteRenderer.h"

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
	bIsSolid = false;
	bDestroyed = false;
}

Actor::Actor(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color, glm::vec3 velocity)
{
	m_position = pos;
	m_scale = size;
	m_velocity = velocity;
	m_color = color;
	Sprite = sprite;

	m_rotAngle = 0.0f;
	m_radius = 1.0f;
	m_rotAxis = glm::vec3(0.0f);

	m_active = true;
	m_delete = false;
	bIsSolid = false;
	bDestroyed = false;
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

void Actor::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(this->Sprite, this->m_position, this->m_scale, this->m_rotAngle, this->m_color);
}

void Actor::SetActive(bool newActive)
{
	m_physicsActor->active = newActive;
}

void Actor::SetDelete(bool newDelete)
{
	m_delete = newDelete;
}

void Actor::SetColor(glm::vec3 color)
{
	m_color = color;
}