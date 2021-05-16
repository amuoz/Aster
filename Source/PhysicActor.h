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
	~PhysicActor();

public:

	bool bSimulate;
	bool bCheckCollision;

	glm::vec3 vel;
	glm::vec3 pos;
	glm::vec3 size;

	float mass;
	glm::vec3 accelerationForce;
	float radius;
	bool bounce;

	std::shared_ptr<Actor> report;

	std::set<std::shared_ptr<PhysicActor> > Collisions;

	// Collision channels
	CollisionChannel Channel;
	std::map<CollisionChannel, CollisionResponse> ChannelResponse;
};
