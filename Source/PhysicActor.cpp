#include "PhysicActor.h"

#include <iostream>

PhysicActor::PhysicActor(CollisionChannel channel)
{
	Channel = channel;
	// default channels all block
	ChannelResponse.insert(std::pair<CollisionChannel, CollisionResponse>(CollisionChannel::STATIC, CollisionResponse::BLOCK));
	ChannelResponse.insert(std::pair<CollisionChannel, CollisionResponse>(CollisionChannel::DYNAMIC, CollisionResponse::BLOCK));
	ChannelResponse.insert(std::pair<CollisionChannel, CollisionResponse>(CollisionChannel::PLAYER, CollisionResponse::BLOCK));

	bCheckCollision = false;

	pos = glm::vec3(0.0f);
	size = glm::vec3(0.0f);

	report = nullptr;
}

PhysicActor::~PhysicActor()
{
	//std::cout << "PhysicActor destroyed" << std::endl;
}
