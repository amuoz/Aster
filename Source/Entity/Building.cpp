#include "Building.h"

#include "Common.h"
#include "Config.h"
#include "Sprite.h"
#include "Physics.h"
#include "PhysicActor.h"
#include <iostream>

Building::Building(glm::vec2 doorPosition,
									 glm::vec3 doorSize,
									 std::unique_ptr<Sprite> doorSprite,
									 std::unique_ptr<Sprite> floorSprite,
									 glm::vec4 floorColor,
									 std::list<std::shared_ptr<Actor> > roomActors)
		: Actor(doorPosition, doorSize, std::move(doorSprite), floorColor)
{
	Actors = roomActors;

	ActorCollider = Physics::Get()->AddDynamicActor(doorPosition, doorSize, CollisionChannel::STATIC);
	ActorCollider->bCheckCollision = true;
	ActorCollider->ChannelResponse[CollisionChannel::STATIC] = CollisionResponse::IGNORED;
	ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::IGNORED;
	ActorCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::OVERLAP;
}

Building::~Building()
{
	//std::cout << "Building destroyed" << std::endl;
}

void Building::Update(float deltaTime, glm::vec4 playerAttackHitbox)
{
	auto iterator = Actors.begin();
	while (iterator != Actors.end())
	{
		if ((*iterator)->IsDestroyed)
		{
			// If actor is only pointed by level list it will be deleted
			(*iterator)->Destroy();
			iterator = Actors.erase(iterator);
		}
		else
		{
			(*iterator)->Update(deltaTime, playerAttackHitbox);
			++iterator;
		}
	}
}

void Building::Draw(SpriteRenderer &renderer, double deltaTime)
{
	for (auto &actor : Actors)
	{
		if (!actor->IsDestroyed)
		{
			actor->Draw(renderer, deltaTime);
		}
	}
}

void Building::OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	std::cout << "Player IN!" << std::endl;
}

void Building::OnEndOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	std::cout << "Player OUT!" << std::endl;
}
