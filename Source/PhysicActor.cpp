#include "PhysicActor.h"
#include <iostream>

PhysicActor::PhysicActor(CollisionChannel channel)
{
	Channel = channel;
	// default channels
	ChannelResponse.insert(std::pair<CollisionChannel, CollisionResponse>(CollisionChannel::STATIC, CollisionResponse::BLOCK));
	ChannelResponse.insert(std::pair<CollisionChannel, CollisionResponse>(CollisionChannel::DYNAMIC, CollisionResponse::BLOCK));
	ChannelResponse.insert(std::pair<CollisionChannel, CollisionResponse>(CollisionChannel::PLAYER, CollisionResponse::BLOCK));

	bSimulate = false;
	bCheckCollision = false;
	vel = glm::vec3(0.0f);
	pos = glm::vec3(0.0f);
	size = glm::vec3(0.0f);

	mass = 0.0f;
	accelerationForce = glm::vec3(0.0f);
	radius = 0.0f;
	bounce = false;

	report = nullptr;
}

PhysicActor::~PhysicActor()
{
	std::cout << "PhysicActor destroyed" << std::endl;
}
