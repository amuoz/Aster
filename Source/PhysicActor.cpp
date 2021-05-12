#include "PhysicActor.h"

PhysicActor::PhysicActor(CollisionChannel channel)
{
	Channel = channel;
	// default channels
	ChannelResponse.insert(std::pair<CollisionChannel, CollisionResponse>(CollisionChannel::STATIC, CollisionResponse::BLOCK));
	ChannelResponse.insert(std::pair<CollisionChannel, CollisionResponse>(CollisionChannel::DYNAMIC, CollisionResponse::BLOCK));
	ChannelResponse.insert(std::pair<CollisionChannel, CollisionResponse>(CollisionChannel::PLAYER, CollisionResponse::BLOCK));
}
