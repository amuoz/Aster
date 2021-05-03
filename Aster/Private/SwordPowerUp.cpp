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

// TODO: calculate this vvv based on the current animation
const int ANIMATION_FRAMES = 18;
const float STILL_CHANCE = 0.3;
const float CHANGE_DIRECTION_CHANCE = 0.2;
const float AGGRO_SIZE = 350;

SwordPowerUp::SwordPowerUp(glm::vec3 pos, glm::vec3 size, Sprite *sprite, glm::vec3 color, glm::vec3 velocity) : Actor(pos, size, sprite, color, velocity)
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

	State = ActorState::IDLE;
	LastState = ActorState::IDLE;
}

SwordPowerUp::~SwordPowerUp()
{
}

void SwordPowerUp::Render(Shader)
{
}

void SwordPowerUp::Update(float deltaTime, glm::vec4 attackHitbox)
{
}

void SwordPowerUp::TakeDamage()
{
}

void SwordPowerUp::OnContact(
		std::shared_ptr<Physics::PhysicActor> external,
		std::shared_ptr<Physics::PhysicActor> internal)
{
	if (internal == ActorCollider && external->report->IsPlayer())
	{
		IsDestroyed = true;
		((Player*)(external->report))->PowerUp(PowerUpType::SWORD);
	}
}