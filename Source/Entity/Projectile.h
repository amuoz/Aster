#pragma once

#include <glm/glm.hpp>
#include "Actor.h"

class Texture2D;
class Sprite;

class Projectile : public Actor
{
public:
	Projectile(glm::vec2 pos, glm::vec3 size, glm::vec2 direction, std::unique_ptr<Sprite> sprite, glm::vec4 color = glm::vec4(1.0f));
	virtual ~Projectile();

	void Update(float, glm::vec4) override;
	void Draw(SpriteRenderer &renderer, double deltaTime) override;

	void OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other) override;
	void OnEndOverlapFunction(std::shared_ptr<PhysicActor> other) override;

private:
	glm::vec2 Direction;

	void DebugProjectileHitbox(SpriteRenderer renderer);
};
