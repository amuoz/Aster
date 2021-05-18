#pragma once

#include <cmath>
#include <vector>
#include <utility>
#include <memory>

#include <glm/glm.hpp>

#include "AsterTypes.h"

#define MAX_DYNAMICS 1000

class PhysicActor;

class Physics
{
public:
	
	Physics(const glm::vec3 &gravity);
	~Physics();

	void Update(float deltaTime);
	void UpdateDymanicPos(std::shared_ptr<PhysicActor> geom, float deltaTime);

	// sphere collision
	bool CheckCircleCircleCollision(const glm::vec3 &circle1Pos, float circle1Radius, const glm::vec3 &circle2Pos, float circle2Radius,
		glm::vec3 &col, glm::vec3 &normal);

	// AABB collision
	bool CheckRectRectCollision(const glm::vec3& rect1Pos, const glm::vec3& rect1Size,
		const glm::vec3& rect2Pos, const glm::vec3& rect2Size, glm::vec3& col);

	std::shared_ptr<PhysicActor> AddDynamicActor(const glm::vec3 &pos, const glm::vec3 &vel, const glm::vec3& size, bool justReport, CollisionChannel channel, glm::vec3 force = glm::vec3(0.0f), float mass = 1.0f);

	void DeleteDynamicActor(std::shared_ptr<PhysicActor> geom);

private:

	void DoCollisions(std::shared_ptr<PhysicActor> geom);

	glm::vec3 m_gravityForce;

	std::vector<std::shared_ptr<PhysicActor> > PhysicsPool;
};
