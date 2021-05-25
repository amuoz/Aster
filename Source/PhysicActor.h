#pragma once

#include <glm/glm.hpp>
#include <map>
#include <set>
#include <memory>
#include <functional>

#include "AsterTypes.h"

using OnBeginOverlapFunction = std::function<void(std::shared_ptr<class PhysicActor>)>;
using OnEndOverlapFunction = std::function<void(std::shared_ptr<class PhysicActor>)>;

class Actor;

class PhysicActor
{
public:

	PhysicActor(CollisionChannel channel);
	~PhysicActor();

public:

	bool bCheckCollision;

	glm::vec3 pos;
	glm::vec3 size;

	std::shared_ptr<Actor> report;

	std::set<std::shared_ptr<PhysicActor> > Collisions;

	// Collision channels
	CollisionChannel Channel;
	std::map<CollisionChannel, CollisionResponse> ChannelResponse;

	OnBeginOverlapFunction OnBeginOverlapPtr;
	OnEndOverlapFunction OnEndOverlapPtr;
};
