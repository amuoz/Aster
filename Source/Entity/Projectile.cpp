#include "Projectile.h"

#include <iostream>

#include "Common.h"
#include "Config.h"
#include "Sprite.h"
#include "Physics.h"
#include "PhysicActor.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

Projectile::Projectile(glm::vec2 pos, glm::vec3 size, glm::vec2 direction, std::unique_ptr<Sprite> sprite, glm::vec4 color)
		: Actor(pos, size, std::move(sprite), color)
{
	ZIndex = 0.1f;
	Direction = direction;

	// std::cout << "pos: " << pos.x << ", " << pos.y << std::endl;
	// std::cout << "Proj size: " << size.x << ", " << size.y << std::endl;
	ActorCollider = Physics::Get()->AddDynamicActor(pos, size, CollisionChannel::DYNAMIC);
	ActorCollider->bCheckCollision = true;
	ActorCollider->ChannelResponse[CollisionChannel::STATIC] = CollisionResponse::BLOCK;
	ActorCollider->ChannelResponse[CollisionChannel::DYNAMIC] = CollisionResponse::BLOCK;
	ActorCollider->ChannelResponse[CollisionChannel::PLAYER] = CollisionResponse::BLOCK;
}

Projectile::~Projectile()
{
	//std::cout << "Projectile destroyed" << std::endl;
}

void Projectile::Update(float deltaTime, glm::vec4 attackHitbox)
{
	Actor::Update(deltaTime, attackHitbox);
	Move(deltaTime, Direction);
}

void Projectile::Draw(SpriteRenderer &renderer, double deltatime)
{
	if (IsActive())
	{
		glm::vec3 spritePosition(Position, ZIndex);
		ActorSprite->Draw(AnimationType::IDLE, renderer, deltatime, spritePosition, Scale, m_rotAngle, Color);
		// DebugProjectileHitbox(renderer);
	}
}

void Projectile::OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other)
{
	std::cout << "TakeDamage!" << std::endl;
	other->report->TakeDamage();
}

void Projectile::OnEndOverlapFunction(std::shared_ptr<PhysicActor> other)
{
}

void Projectile::DebugProjectileHitbox(SpriteRenderer renderer)
{
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Texture2D texture = ResourceManager::GetInstance()->GetTexture("block");
	renderer.DrawTexture(
			texture,
			glm::vec3(Position.x, Position.y, 9999.9f),
			glm::vec2(Scale.x, Scale.y),
			0.0f,
			glm::vec4(0.8f, 0.0f, 0.2f, 1.0f));
}
