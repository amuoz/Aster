#pragma once

#include <glm/glm.hpp>
#include "Actor.h"

#include "Sprite.h"

enum class AnimationType;

class Player : public Actor
{
public:
	Player(glm::vec3 pos, glm::vec3 size, Sprite *sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	~Player();

	void Render(Shader shader) override;
	void Update(float, glm::vec4) override;
	void Draw(SpriteRenderer &renderer, double deltaTime) override;
	void TakeDamage() override;
	void OnContact(
			std::shared_ptr<Physics::PhysicActor> external,
			std::shared_ptr<Physics::PhysicActor> internal) override;
	bool IsPlayer() override;

	void Idle();
	void Attack();
	glm::vec4 GetAttackHitbox();

private:
	AnimationType CurrentAnimation;
};