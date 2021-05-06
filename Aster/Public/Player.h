#pragma once

#include <list>

#include <glm/glm.hpp>
#include "Actor.h"
#include "Sprite.h"

enum class AnimationType;
enum class PowerUp;

class Player : public Actor
{
public:
	Player(glm::vec3 pos, glm::vec3 size, Sprite *sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	~Player();

	void Render(Shader shader) override;
	void Update(float, glm::vec4) override;
	void Draw(SpriteRenderer &renderer, double deltaTime) override;
	void TakeDamage() override;
	void Move(float deltaTime, glm::vec3 direction) override;
	void OnContact(
			std::shared_ptr<Physics::PhysicActor> external,
			std::shared_ptr<Physics::PhysicActor> internal) override;
	bool IsPlayer() override;

	void Idle();
	void Attack();
	glm::vec4 GetAttackHitbox();
	void PowerUp(PowerUpType powerUp);
	std::list<PowerUpType> GetPowerUps();
	PowerUpType GetActivePowerUp();

private:
	std::list<PowerUpType> Inventory;
	AnimationType CurrentAnimation;
	PowerUpType ActivePowerUp = PowerUpType::SPEAR;

	AnimationType GetDefaultAnimation();
	AnimationType GetSwordAnimation();
};
