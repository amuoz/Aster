#include "Projectile.h"

#include "Common.h"
#include "Config.h"
#include "Sprite.h"
#include "Physics.h"
#include "PhysicActor.h"
#include <iostream>

Projectile::Projectile(glm::vec2 pos, glm::vec3 size, glm::vec2 speed, std::unique_ptr<Sprite> sprite, glm::vec4 color)
		: Actor(pos, size, std::move(sprite), color)
{
	ZIndex = 0.1f;
}

Projectile::~Projectile()
{
	//std::cout << "Projectile destroyed" << std::endl;
}

void Projectile::Draw(SpriteRenderer &renderer, double deltatime)
{
	if (IsActive())
	{
		glm::vec3 spritePosition(Position, ZIndex);
		ActorSprite->Draw(AnimationType::IDLE, renderer, deltatime, spritePosition, m_scale, m_rotAngle, Color);
	}
}
