#pragma once

#include <glm/glm.hpp>
#include <map>
#include <set>
#include <memory>

#include "AsterTypes.h"

class Actor;

class PhysicActor
{
public:

	PhysicActor(CollisionChannel channel);

public:

	bool active = false;

	bool bSimulate = false;
	bool bCheckCollision = false;

	glm::vec3 vel = glm::vec3(0.0f);
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 size = glm::vec3(0.0f);

	float mass = 0.0f;
	glm::vec3 accelerationForce = glm::vec3(0.0f);

	Actor* report = nullptr;

	float radius = 0.0f;
	bool bounce = false;
	bool justReport = false;

	std::set<std::shared_ptr<PhysicActor> > Collisions;

	// Collision channels
	CollisionChannel Channel;
	std::map<CollisionChannel, CollisionResponse> ChannelResponse;
	
};
