#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "Actor.h"
#include "Sprite.h"
#include "Physics.h"

enum class AnimationType;

class SpikeEnemy : public Actor
{
public:
	SpikeEnemy(glm::vec3 pos, glm::vec3 size, Sprite* sprite, float framePeriod, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	~SpikeEnemy();

	void Render(Shader shader) override;
	void Update(float, glm::vec4) override;
	void Draw(SpriteRenderer &renderer, double deltaTime) override;
	void TakeDamage() override;
	void OnContact(
			std::shared_ptr<Physics::PhysicActor> external,
			std::shared_ptr<Physics::PhysicActor> internal) override;

private:
	bool PassRandomChance(float chance);
	float GetRandomDirectionComponent();
	glm::vec3 GetRandomDirection();
	void SetWanderMovement();
	void SetSpeed();
	glm::vec3 GetAggroPosition(glm::vec3 actorPosition, glm::vec3 actorSize);

	float AnimationPeriod;
	AnimationType CurrentAnimation;
	glm::vec3 Direction;
	float ChangeDirectionPeriod;
	float LastDirectionChange;
	float AnimationProgress;
	const float MAX_SPEED = 100.0f;

	std::shared_ptr<Physics::PhysicActor> AggroCollider = nullptr;
};
