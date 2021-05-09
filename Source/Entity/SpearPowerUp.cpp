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

SpearPowerUp::SpearPowerUp(glm::vec3 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color, glm::vec3 velocity) : Actor(pos, size, std::move(sprite), color, velocity)
{
	ActorCollider = g_PhysicsPtr->AddDynamicActor(
			pos,
			velocity,
			size,
			false,
			glm::vec3(0.0f),
			1.0f);
	ActorCollider->bCheckCollision = true;
	ActorCollider->report = this;
}

SpearPowerUp::~SpearPowerUp()
{
}

void SpearPowerUp::OnContact(
		std::shared_ptr<Physics::PhysicActor> external,
		std::shared_ptr<Physics::PhysicActor> internal)
{
	if (internal == ActorCollider && external->report->IsPlayer())
	{
		IsDestroyed = true;
		((Player*)(external->report))->PowerUp(PowerUpType::SPEAR);
	}
}
