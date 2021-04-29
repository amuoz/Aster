#pragma once

#include <glm/glm.hpp>
#include "Actor.h"
#include "ICircleContactReport.h"

#include "Sprite.h"

enum class AnimationType;

class SpikeEnemy : public Actor, ICircleContactReport
{
public:
	SpikeEnemy(glm::vec3 pos, glm::vec3 size, Sprite* sprite, float framePeriod, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	~SpikeEnemy();

	void Render(Shader shader) override;
	void Update(float, glm::vec4) override;
	void Draw(SpriteRenderer &renderer, double deltaTime) override;
	void TakeDamage() override;

	void OnContact(Physics::PhysicActor* other) override;

private:
	bool PassRandomChance(float chance);
	float GetRandomDirectionComponent();
	glm::vec3 GetRandomDirection();
	void SetWanderMovement();
	void SetSpeed();

	float AnimationPeriod;
	AnimationType CurrentAnimation;
	glm::vec3 Direction;
	float ChangeDirectionPeriod;
	float LastDirectionChange;
	float AnimationProgress;
	const float MAX_SPEED = 100.0f;
};
