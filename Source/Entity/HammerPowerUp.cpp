#include "HammerPowerUp.h"

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

HammerPowerUp::HammerPowerUp(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec4 color, glm::vec3 velocity) : Actor(pos, size, std::move(sprite), color)
{
	ActorCollider = Physics::Get()->AddDynamicActor(pos, size, CollisionChannel::DYNAMIC);
			CollisionChannel::DYNAMIC,
	ActorCollider->bCheckCollision = true;
	ActorCollider->ChannelResponse[CollisionChannel::STATIC] = CollisionResponse::IGNORED;
	ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::IGNORED;
	ActorCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::OVERLAP;
}

HammerPowerUp::~HammerPowerUp()
{
	//std::cout << "HammerPowerUp destroyed" << std::endl;
}

void HammerPowerUp::OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	if (!IsDestroyed && other->report->IsPlayer())
	{
		IsDestroyed = true;
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(other->report);
		player->PowerUp(PowerUpType::HAMMER);
	}
}

void HammerPowerUp::OnEndOverlapFunction(std::shared_ptr<PhysicActor> other)
{

}
