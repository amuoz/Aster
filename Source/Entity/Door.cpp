#include "Door.h"

#include "Common.h"
#include "Config.h"
#include "Sprite.h"
#include "Physics.h"
#include "PhysicActor.h"
#include <iostream>

Door::Door(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec4 color): Actor(pos, size, std::move(sprite), color)
{
	ZIndex = 0.1f;
	ActorCollider = Physics::Get()->AddDynamicActor(pos, size, CollisionChannel::STATIC);
	ActorCollider->bCheckCollision = true;
	ActorCollider->ChannelResponse[CollisionChannel::STATIC] = CollisionResponse::IGNORED;
	ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::IGNORED;
	ActorCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::OVERLAP;
}

Door::~Door()
{
	//std::cout << "Door destroyed" << std::endl;
}

void Door::OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	std::cout << "Player IN!" << std::endl;
}

void Door::OnEndOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	std::cout << "Player OUT!" << std::endl;
}
