#include "SwordPowerUp.h"

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

SwordPowerUp::SwordPowerUp(glm::vec3 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color, glm::vec3 velocity) : Actor(pos, size, std::move(sprite), color, velocity)
{
	ActorCollider = g_PhysicsPtr->AddDynamicActor(
			pos,
			velocity,
			size,
			false,
			CollisionChannel::DYNAMIC,
			glm::vec3(0.0f),
			1.0f);
	ActorCollider->bCheckCollision = true;
	ActorCollider->report = this;
	ActorCollider->ChannelResponse[CollisionChannel::STATIC] = CollisionResponse::IGNORE_C;
	ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::IGNORE_C;
	ActorCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::OVERLAP;
}

SwordPowerUp::~SwordPowerUp()
{
}

void SwordPowerUp::OnContact(
		std::shared_ptr<PhysicActor> external,
		std::shared_ptr<PhysicActor> internal)
{
	if (!IsDestroyed && internal == ActorCollider && external->report->IsPlayer())
	{
		IsDestroyed = true;
		((Player *)(external->report))->PowerUp(PowerUpType::SWORD);
	}
}
