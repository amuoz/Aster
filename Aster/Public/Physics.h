#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAX_DYNAMICS 1000

class ICircleContactReport;

class Physics
{
public:
	
	struct PhysicActor
	{
		bool active = false;

		bool bSimulate = false;
		bool bCheckCollision = false;

		glm::vec3 vel = glm::vec3(0.0f);
		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 size = glm::vec3(0.0f);

		float mass = 0.0f;
		glm::vec3 accelerationForce = glm::vec3(0.0f);

		ICircleContactReport* report = nullptr;

		float radius = 0.0f;
		bool bounce = false;
		bool justReport = false;
	};

	Physics(const glm::vec3 &gravity);
	~Physics();

	void Update(float deltaTime);
	void UpdateDymanicPos(PhysicActor &geom, float deltaTime);

	// sphere collision
	bool CheckCircleCircleCollision(const glm::vec3 &circle1Pos, float circle1Radius, const glm::vec3 &circle2Pos, float circle2Radius,
		glm::vec3 &col, glm::vec3 &normal);

	// AABB collision
	bool CheckRectRectCollision(const glm::vec3& rect1Pos, const glm::vec3& rect1Size,
		const glm::vec3& rect2Pos, const glm::vec3& rect2Size, glm::vec3& col);

	PhysicActor* AddDynamicActor(const glm::vec3 &pos, const glm::vec3 &vel, const glm::vec3& size, bool justReport, glm::vec3 force = glm::vec3(0.0f), float mass = 1.0f);

	void DeleteDynamicActor(PhysicActor *geom);

private:

	void DoCollisions(PhysicActor& geom);

	glm::vec3 m_gravityForce;

	std::vector<PhysicActor*> m_dynamicActors;
};

#endif // !PHYSICS_H
