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
									 std::list<std::shared_ptr<Actor> > exteriorActors,
									 std::list<std::shared_ptr<Actor> > interiorActors)
		: Actor(doorPosition, doorSize, std::move(doorSprite), floorColor)
{
	ExteriorActors = exteriorActors;
	InteriorActors = interiorActors;
	ShowInterior = false;

	ActorCollider = Physics::Get()->AddDynamicActor(doorPosition, doorSize, CollisionChannel::STATIC);
	ActorCollider->bCheckCollision = true;
	ActorCollider->ChannelResponse[CollisionChannel::STATIC] = CollisionResponse::IGNORED;
	ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::IGNORED;
	ActorCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::OVERLAP;

	EntryCollider = Physics::Get()->AddDynamicActor(GetEntryPosition(doorPosition), GetEntrySize(doorSize), CollisionChannel::STATIC);
	EntryCollider->bCheckCollision = true;
	EntryCollider->ChannelResponse[CollisionChannel::STATIC] = CollisionResponse::IGNORED;
	EntryCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::IGNORED;
	EntryCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::OVERLAP;
	EntryCollider->OnBeginOverlapPtr = [=](std::shared_ptr<PhysicActor> other) {
		this->OnBeginOverlapEntry(other);
	};
	EntryCollider->OnEndOverlapPtr = [=](std::shared_ptr<PhysicActor> other) {
		this->OnEndOverlapEntry(other);
	};
}

Building::~Building()
{
	//std::cout << "Building destroyed" << std::endl;
	Physics::Get()->DeleteDynamicActor(EntryCollider);
}

glm::vec2 Building::GetEntryPosition(glm::vec2 doorPosition)
{
	return glm::vec2(doorPosition.x, doorPosition.y + POSITION_OFFSET);
}

glm::vec3 Building::GetEntrySize(glm::vec3 doorSize)
{
	return glm::vec3(doorSize.x, POSITION_OFFSET, doorSize.z);
}

void Building::BeginPlay()
{
	EntryCollider->report = shared_from_this();

	for (auto &actor : ExteriorActors)
	{
		actor->BeginPlay();
	}

	for (auto &actor : InteriorActors)
	{
		actor->BeginPlay();
	}

	Actor::BeginPlay();
}

void Building::Update(float deltaTime, glm::vec4 playerAttackHitbox)
{
	auto iterator = ExteriorActors.begin();
	while (iterator != ExteriorActors.end())
	{
		if ((*iterator)->IsDestroyed)
		{
			// If actor is only pointed by level list it will be deleted
			(*iterator)->Destroy();
			iterator = ExteriorActors.erase(iterator);
		}
		else
		{
			(*iterator)->Update(deltaTime, playerAttackHitbox);
			++iterator;
		}
	}

	iterator = InteriorActors.begin();
	while (iterator != InteriorActors.end())
	{
		if ((*iterator)->IsDestroyed)
		{
			// If actor is only pointed by level list it will be deleted
			(*iterator)->Destroy();
			iterator = InteriorActors.erase(iterator);
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
	auto sortByPosition = [](std::shared_ptr<Actor> a, std::shared_ptr<Actor> b)
	{
		return a->GetPosition().y < b->GetPosition().y;
	};
	// Sort sprites by z-index
	ExteriorActors.sort(sortByPosition);
	InteriorActors.sort(sortByPosition);

	if (ShowInterior) {
		for (auto &actor : InteriorActors)
		{
			if (!actor->IsDestroyed)
			{
				actor->Draw(renderer, deltaTime);
			}
		}
	}

	for (auto &actor : ExteriorActors)
	{
		if (!actor->IsDestroyed)
		{
			if (ShowInterior)
			{
				actor->SetAlpha(CEILING_ALPHA);
			}
			else
			{
				actor->SetAlpha(FULL_ALPHA);
			}
			actor->Draw(renderer, deltaTime);
		}
	}
}

void Building::OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	hasEntererd = true;
}

void Building::OnEndOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	hasEntererd = false;

	if (IsEntering)
	{
		ShowInterior = false;
	}
}

void Building::OnBeginOverlapEntry(std::shared_ptr<PhysicActor> other)
{
	IsEntering = true;
}

void Building::OnEndOverlapEntry(std::shared_ptr<PhysicActor> other)
{
	IsEntering = false;

	if (hasEntererd)
	{
		ShowInterior = true;
	}
}
