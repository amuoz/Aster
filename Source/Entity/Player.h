#pragma once

#include <list>
#include <vector>
#include <glm/glm.hpp>

#include "Actor.h"
#include "AsterTypes.h"

class Sprite;

class Player : public Actor
{
public:
	Player(glm::vec3 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color = glm::vec3(1.0f));
	~Player();

	void Update(float, glm::vec4) override;
	void Draw(SpriteRenderer &renderer, double deltaTime) override;
	void TakeDamage() override;
	void SetInputDirection(glm::vec3 direction);
	void Dash(glm::vec3 direction);
	void SetDashSpeed();
	void SetDashIFrames();

	bool IsPlayer() override;
	void SetState(ActorState state) override;

	void Idle();
	void Attack();
	glm::vec4 GetAttackHitbox();
	void PowerUp(PowerUpType powerUp);
	std::vector<PowerUpType> GetPowerUps();
	PowerUpType GetActivePowerUp();
	void SelectPowerUp(unsigned int index);

private:
	std::vector<PowerUpType> Inventory;
	AnimationType CurrentAnimation;
	PowerUpType ActivePowerUp;
	float DashTime;
	glm::vec3 InputDirection;
	glm::vec3 MovementDirection;
	glm::vec3 LastMovementDirection;

	void Move(float deltaTime, glm::vec3 direction) override;
	AnimationType GetAnimationFromState();
	bool IsInDashIFrames();
	bool IsAttackAnimationPlaying();
	bool IsDashState();
	bool IsBlockedByHammer();
	bool IsHammerAttack();

};
