#include "SpearPowerUp.h"

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Sprite.h"
#include "Player.h"
#include "Physics.h"
#include "SpriteRenderer.h"
#include "PhysicActor.h"

SpearPowerUp::SpearPowerUp(glm::vec3 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color) : Actor(pos, size, std::move(sprite), color)
{
	ActorCollider = Physics::Get()->AddDynamicActor(pos, size, CollisionChannel::DYNAMIC);
	ActorCollider->bCheckCollision = true;
	ActorCollider->ChannelResponse[CollisionChannel::STATIC] = CollisionResponse::IGNORED;
	ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::IGNORED;
	ActorCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::OVERLAP;
}

SpearPowerUp::~SpearPowerUp()
{
	//std::cout << "SpearPowerUp destroyed" << std::endl;
}

void SpearPowerUp::OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	if (!IsDestroyed && other->report->IsPlayer())
	{
		IsDestroyed = true;
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(other->report);
		player->PowerUp(PowerUpType::SPEAR);
	}
}

void SpearPowerUp::OnEndOverlapFunction(std::shared_ptr<PhysicActor> other)
{

}
