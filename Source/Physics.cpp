#include "Physics.h"

#include <iostream>
#include <algorithm>
#include <utility>
#include <memory>

#include "Entity/Actor.h"
#include "PhysicActor.h"

// Instantiate static variables
std::shared_ptr<Physics> Physics::Instance = nullptr;

Physics::Physics()
{
}

Physics::~Physics()
{
	DeleteAllDynamics();
}

void Physics::Update(float deltaTime)
{
	for (auto& dynamicActor : PhysicsPool)
	{
		// check collisions
		if (dynamicActor->bCheckCollision)
		{
			DoCollisions(dynamicActor);
		}
	}
}

bool Physics::CheckRectRectCollision(const glm::vec3& rect1Pos, const glm::vec3& rect1Size, 
	const glm::vec3& rect2Pos, const glm::vec3& rect2Size, glm::vec3& col) // AABB - AABB collision
{
	// collision x-axis?
	float xDist1 = rect1Pos.x + rect1Size.x - rect2Pos.x;
	float xDist2 = rect2Pos.x + rect2Size.x - rect1Pos.x;
	//bool collisionX = rect1Pos.x + rect1Size.x >= rect2Pos.x && rect2Pos.x + rect2Size.x >= rect1Pos.x;
	bool collisionX = xDist1 > 0 && xDist2 > 0;
	
	// collision y-axis?
	float yDist1 = rect1Pos.y - (rect2Pos.y - rect2Size.y);
	float yDist2 = rect2Pos.y - (rect1Pos.y - rect1Size.y);
	//bool collisionY = rect1Pos.y >= rect2Pos.y - rect2Size.y && rect2Pos.y >= rect1Pos.y - rect1Size.y;
	bool collisionY = yDist1 > 0 && yDist2 > 0;

	// collision only if on both axes
	if (collisionX && collisionY)
	{
		col = rect1Pos;
		// use lowest distance to resolve the collision
		float distances[] = { xDist1, xDist2, yDist1, yDist2 };
		float minDistance = *std::min_element(distances, distances+4);
		if (xDist1 == minDistance)
		{
			col.x = rect1Pos.x - xDist1;
		}
		else if(xDist2 == minDistance)
		{
			col.x = rect1Pos.x + xDist2;
		}
		else if (yDist1 == minDistance)
		{
			col.y = rect1Pos.y - yDist1;
		}
		else
		{
			col.y = rect1Pos.y + yDist2;
		}

		return true;
	}

	return false;
}

std::shared_ptr<PhysicActor> Physics::AddDynamicActor(const glm::vec3 &pos, const glm::vec3& size, CollisionChannel channel)
{
	auto geom = std::make_shared<PhysicActor>(channel);
	geom->pos = pos;
	geom->size = size;

	PhysicsPool.push_back(geom);

	return geom;
}

std::vector<std::shared_ptr<PhysicActor>>::iterator Physics::DeleteDynamicActor(std::shared_ptr<PhysicActor> geom)
{
	// Notify end overlap to its active collisions
	for (auto iterator = geom->Collisions.begin(); iterator != geom->Collisions.end(); ++iterator)
	{
		if ((*iterator)->OnEndOverlapPtr != nullptr)
		{
			(*iterator)->OnEndOverlapPtr(geom);
		}
	}

	auto it = std::find(PhysicsPool.begin(), PhysicsPool.end(), geom);
	if (it != PhysicsPool.end())
	{ 
		(*it)->report.reset();
		return PhysicsPool.erase(it);
	}
}

void Physics::DeleteAllDynamics()
{
	auto iterator = PhysicsPool.begin();
	while (iterator != PhysicsPool.end())
	{
		iterator = DeleteDynamicActor(*iterator);
	}
	PhysicsPool.clear();
}

std::shared_ptr<Physics> Physics::Get()
{
	if (Instance == nullptr)
	{
		// Cant use make_shared because constructor is private for singleton support
		//Instance = std::make_shared<Physics>();
		Instance = std::shared_ptr<Physics>(new Physics());
	}
	return Instance;
}

void Physics::DoCollisions(std::shared_ptr<PhysicActor> geom)
{
	glm::vec3 col, normal;
	for (auto& dynamicActor : PhysicsPool)
	{
		if (geom != dynamicActor && (geom->report != dynamicActor->report))
		{
			CollisionResponse myResponse = geom->ChannelResponse[dynamicActor->Channel];
			CollisionResponse otherResponse = dynamicActor->ChannelResponse[geom->Channel];
			
			// If any collision in ignore then continue with next one
			if (myResponse == CollisionResponse::IGNORED || otherResponse == CollisionResponse::IGNORED)
			{
				continue;
			}

			if (CheckRectRectCollision(geom->pos, geom->size, dynamicActor->pos, dynamicActor->size, col))
			{
				// If both blocking then resolve collision
				if (myResponse == CollisionResponse::BLOCK && otherResponse == CollisionResponse::BLOCK)
				{
					geom->pos = col;
				}

				// Notify overlap
				// Each collision will take care of his own overlap
				if(geom->report && myResponse == CollisionResponse::OVERLAP && otherResponse != CollisionResponse::IGNORED)
				{
					// Begin overlap just once
					auto it = std::find(geom->Collisions.begin(), geom->Collisions.end(), dynamicActor);
					if (it == geom->Collisions.end())
					{
						geom->Collisions.insert(dynamicActor);
						if (geom->OnBeginOverlapPtr != nullptr)
						{
							geom->OnBeginOverlapPtr(dynamicActor);
						}
					}
					
				}
			}
			else
			{
				auto it = std::find(geom->Collisions.begin(), geom->Collisions.end(), dynamicActor);
				if (it != geom->Collisions.end())
				{
					if (geom->OnEndOverlapPtr != nullptr)
					{
						geom->OnEndOverlapPtr(dynamicActor);
					}
					geom->Collisions.erase(dynamicActor);	
				}
				
			}
		}
	}
}
