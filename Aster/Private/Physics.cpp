#include "Physics.h"

#include "ICircleContactReport.h"

#include <iostream>
#include <algorithm>

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
		// simulate physics
		if (dynamicActor->bSimulate)
		{
			UpdateDymanicPos(*dynamicActor, deltaTime);
		}

		// check collisions
		if (dynamicActor->bCheckCollision)
		{
			DoCollisions(*dynamicActor);
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

		// Desplazar la esfera A que ha colisionado en la direccion de colision lo suficiente para que los dos radios est�n separados
		col = circle2Pos + normal * (circle1Radius + circle2Radius);
		return true;
	}

	return false;
}

bool Physics::CheckRectRectCollision(const glm::vec3& rect1Pos, const glm::vec3& rect1Size, 
	const glm::vec3& rect2Pos, const glm::vec3& rect2Size, glm::vec3& col) // AABB - AABB collision
{
	// collision x-axis?
	float xDist1 = rect1Pos.x + rect1Size.x - rect2Pos.x;
	float xDist2 = rect2Pos.x + rect2Size.x - rect1Pos.x;
	//bool collisionX = rect1Pos.x + rect1Size.x >= rect2Pos.x && rect2Pos.x + rect2Size.x >= rect1Pos.x;
	bool collisionX = xDist1 > 0 && xDist2 > 0;
	
	// collision y-axis?
	float yDist1 = rect1Pos.y + rect1Size.y - rect2Pos.y;
	float yDist2 = rect2Pos.y + rect2Size.y - rect1Pos.y;
	//bool collisionY = rect1Pos.y + rect1Size.y >= rect2Pos.y && rect2Pos.y + rect2Size.y >= rect1Pos.y;
	bool collisionY = yDist1 > 0 && yDist2 > 0;

	// collision only if on both axes
	if (collisionX && collisionY)
	{
		col = rect1Pos;
		// use lowest distance to resolve the collision
		float distances[] = { xDist1, xDist2, yDist1, yDist2 };
		float minDistance = *std::min_element(distances, distances+4);
		if (xDist1 == minDistance)
		{
			col.x = rect1Pos.x - xDist1;
		}
		else if(xDist2 == minDistance)
		{
			col.x = rect1Pos.x + xDist2;
		}
		else if (yDist1 == minDistance)
		{
			col.y = rect1Pos.y - yDist1;
		}
		else
		{
			col.y = rect1Pos.y + yDist2;
		}

		return true;
	}

	return false;
}

Physics::PhysicActor* Physics::AddDynamicActor(const glm::vec3 &pos, const glm::vec3 &vel, const glm::vec3& size, bool justReport, glm::vec3 force, float mass)
{
	if (m_dynamicActors.size() < MAX_DYNAMICS)
	{
		PhysicActor *geom = new PhysicActor;
		geom->active = false;
		geom->pos = pos;
		geom->vel = vel;
		geom->size = size;
		geom->accelerationForce = force;
		geom->mass = mass;
		geom->radius = size.x / 2;
		geom->justReport = justReport;
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

void Physics::DoCollisions(PhysicActor& geom)
{
	glm::vec3 col, normal;
	for (PhysicActor* dynamicActor : m_dynamicActors)
	{
		//if (&geom != dynamicActor && dynamicActor->active)
		if (&geom != dynamicActor)
		{
			//if (CheckCircleCircleCollision(geom.pos, geom.radius, dynamicActor->pos, dynamicActor->radius, col, normal))
			if (CheckRectRectCollision(geom.pos, geom.size, dynamicActor->pos, dynamicActor->size, col))
			{
				// push actor in normal direction
				if (geom.bounce && dynamicActor->bounce)
				{
					geom.vel = normal * glm::length(geom.vel);
					dynamicActor->vel = -normal * glm::length(dynamicActor->vel);
				}

				if (!geom.justReport && !dynamicActor->justReport)
				{
					geom.pos = col;
				}

				// notify collision
				if (geom.report)
				{
					geom.report->OnContact(dynamicActor);
				}
				//if (dynamicActor->active && dynamicActor->report)
				if (dynamicActor->report)
				{
					dynamicActor->report->OnContact(&geom);
				}
			}
		}
	}
}
