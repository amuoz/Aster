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
		bool active;

		glm::vec3 vel;
		glm::vec3 pos;

		float mass;
		glm::vec3 accelerationForce;

		ICircleContactReport* report = 0;

		float radius;
		bool ignoreContact = false;
	};

	Physics(const glm::vec3 &gravity);
	~Physics();

	void Update(float deltaTime);
	void UpdateDymanicPos(PhysicActor &geom, float deltaTime);

	// sphere collision
	bool CheckCircleCircleCollision(const glm::vec3 &circle1Pos, float circle1Radius, const glm::vec3 &circle2Pos, float circle2Radius,
		glm::vec3 &col, glm::vec3 &normal);

	PhysicActor* AddDynamicActor(const glm::vec3 &pos, const glm::vec3 &vel, float radius, glm::vec3 force = glm::vec3(0.0f), float mass = 1.0f);

	void DeleteDynamicActor(PhysicActor *geom);

private:

	glm::vec3 m_gravityForce;

	std::vector<PhysicActor*> m_dynamicActors;
};

#endif // !PHYSICS_H
