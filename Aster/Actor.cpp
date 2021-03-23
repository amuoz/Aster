#include "Actor.h"

#include "Common.h"

Actor::Actor()
{
	m_active = true;
	m_delete = false;
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