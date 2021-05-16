#include "SpearPowerUp.h"

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Common.h"
#include "Sprite.h"
#include "Player.h"
#include "Physics.h"
#include "SpriteRenderer.h"
#include "PhysicActor.h"

SpearPowerUp::SpearPowerUp(glm::vec3 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color, glm::vec3 velocity) : Actor(pos, size, std::move(sprite), color, velocity)
{
	ActorCollider = g_PhysicsPtr->AddDynamicActor(pos, velocity, size, CollisionChannel::DYNAMIC);
	ActorCollider->bCheckCollision = true;
	//ActorCollider->report = shared_from_this();
	ActorCollider->ChannelResponse[CollisionChannel::STATIC] = CollisionResponse::IGNORE_C;
	ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::IGNORE_C;
	ActorCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::OVERLAP;
}

SpearPowerUp::~SpearPowerUp()
{
	std::cout << "SpearPowerUp destroyed" << std::endl;
}

void SpearPowerUp::OnContact(
		std::shared_ptr<PhysicActor> external,
		std::shared_ptr<PhysicActor> internal)
{
	if (!IsDestroyed && internal == ActorCollider && external->report->IsPlayer())
	{
		IsDestroyed = true;
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(external->report);
		player->PowerUp(PowerUpType::SPEAR);
		//((Player *)(external->report))->PowerUp(PowerUpType::SPEAR);
	}
}
