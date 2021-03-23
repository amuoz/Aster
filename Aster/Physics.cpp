#include "Physics.h"

#include "ICircleContactReport.h"

#include <iostream>

Physics::Physics(const glm::vec3 &gravity)
{
	m_gravityForce = gravity;
}

Physics::~Physics()
{
	for (PhysicActor* dynamicActor: m_dynamicActors)
	{
		delete dynamicActor;
	}

	m_dynamicActors.clear();
	// deallocating the memory
	std::vector<PhysicActor*>().swap(m_dynamicActors);
}

void Physics::Update(float deltaTime)
{
	for (PhysicActor* dynamicActor : m_dynamicActors)
	{
		if (dynamicActor->active)
		{
			UpdateDymanicPos(*dynamicActor, deltaTime);
		}
	}

}

void Physics::UpdateDymanicPos(PhysicActor &geom, float deltaTime)
{
	// F = m * a
	// a = F / m
	// V = V0 + a * t
	// P = Po + V * t
	geom.vel += m_gravityForce * deltaTime;
	geom.vel += (geom.accelerationForce / geom.mass) * deltaTime;
	glm::vec3 prevPos = geom.pos;
	glm::vec3 newPos = geom.pos + geom.vel * deltaTime;
	glm::vec3 desplDir = newPos - prevPos;
	desplDir = glm::normalize(desplDir);
	geom.pos = newPos;

	glm::vec3 col, normal;
	for (PhysicActor* dynamicActor : m_dynamicActors)
	{
		if (&geom != dynamicActor && dynamicActor->active)
		{
			if (CheckCircleCircleCollision(geom.pos, geom.radius, dynamicActor->pos, dynamicActor->radius, col, normal))
			{
				// push actor in normal direction
				if (geom.ignoreContact && dynamicActor->ignoreContact)
				{
					geom.vel = normal * glm::length(geom.vel);
					dynamicActor->vel = -normal * glm::length(dynamicActor->vel);
				}
								
				geom.pos = col;
				
				// notify collision
				if (geom.report)
				{
					geom.report->OnContact(dynamicActor);
				}
				if (dynamicActor->active && dynamicActor->report)
				{
					dynamicActor->report->OnContact(&geom);
				}
			}
		}
	}

}

bool Physics::CheckCircleCircleCollision(const glm::vec3& circle1Pos, float circle1Radius, const glm::vec3& circle2Pos,
	float circle2Radius, glm::vec3& col, glm::vec3& normal)
{
	// Collision if distance between sphere centers is less than radius sum
	float centerDist = glm::distance(circle1Pos, circle2Pos);
	float radiusSum = circle1Radius + circle2Radius;
	if (centerDist < radiusSum) {
		normal = circle1Pos - circle2Pos;
		normal = glm::normalize(normal);

		// Desplazar la esfera A que ha colisionado en la direccion de colision lo suficiente para que los dos radios estén separados
		col = circle2Pos + normal * (circle1Radius + circle2Radius);
		return true;
	}

	return false;
}

Physics::PhysicActor* Physics::AddDynamicActor(const glm::vec3 &pos, const glm::vec3 &vel, float radius, glm::vec3 force, float mass)
{
	if (m_dynamicActors.size() < MAX_DYNAMICS)
	{
		PhysicActor *geom = new PhysicActor;
		geom->active = false;
		geom->pos = pos;
		geom->vel = vel;
		geom->accelerationForce = force;
		geom->mass = mass;
		geom->radius = radius;
		m_dynamicActors.push_back(geom);
		return geom;
	}
	return 0;
}

void Physics::DeleteDynamicActor(PhysicActor *geom)
{
	auto it = std::find(m_dynamicActors.begin(), m_dynamicActors.end(), geom);
	if (it != m_dynamicActors.end())
	{ 
		m_dynamicActors.erase(it);
	}
}
