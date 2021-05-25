#pragma once

#include <cmath>
#include <vector>
#include <utility>
#include <memory>

#include <glm/glm.hpp>

#include "AsterTypes.h"

class PhysicActor;

class Physics
{
public:

	~Physics();

	void Update(float deltaTime);

	// AABB collision
	bool CheckRectRectCollision(const glm::vec3& rect1Pos, const glm::vec3& rect1Size,
		const glm::vec3& rect2Pos, const glm::vec3& rect2Size, glm::vec3& col);

	std::shared_ptr<PhysicActor> AddDynamicActor(const glm::vec3 &pos, const glm::vec3& size, CollisionChannel channel);
	std::vector<std::shared_ptr<PhysicActor>>::iterator DeleteDynamicActor(std::shared_ptr<PhysicActor> geom);

	void DeleteAllDynamics();

	static std::shared_ptr<Physics> Get();

private:
	// private because singleton
	Physics();

	static std::shared_ptr<Physics> Instance;

	void DoCollisions(std::shared_ptr<PhysicActor> geom);

	std::vector<std::shared_ptr<PhysicActor> > PhysicsPool;
};
