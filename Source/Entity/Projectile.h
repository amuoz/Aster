#pragma once

#include <glm/glm.hpp>
#include "Actor.h"

class Texture2D;
class Sprite;

class Projectile : public Actor
{
public:
	Projectile(glm::vec2 pos, glm::vec3 size, glm::vec2 speed, std::unique_ptr<Sprite> sprite, glm::vec4 color = glm::vec4(1.0f));
	virtual ~Projectile();

	void Draw(SpriteRenderer &renderer, double deltaTime) override;

private:
};
