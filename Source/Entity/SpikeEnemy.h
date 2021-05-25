#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "Actor.h"
#include "Physics.h"

enum class AnimationType;
class Sprite;

class SpikeEnemy : public Actor
{
public:
	SpikeEnemy(glm::vec3 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, float framePeriod, glm::vec3 color = glm::vec3(1.0f));
	~SpikeEnemy();

	void BeginPlay() override;
	void Update(float, glm::vec4) override;
	void Draw(SpriteRenderer &renderer, double deltaTime) override;
	void Destroy() override;

	void OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other) override;
	void OnEndOverlapFunction(std::shared_ptr<PhysicActor> other) override;

	void OnBeginOverlapAggro(std::shared_ptr<PhysicActor> other);
	void OnEndOverlapAggro(std::shared_ptr<PhysicActor> other);

private:
	bool PassRandomChance(float chance);
	float GetRandomDirectionComponent();
	glm::vec3 GetRandomDirection();
	void SetWanderMovement();
	void SetAggroMovement();
	void SetSpeed();
	glm::vec3 GetAggroPosition(glm::vec3 actorPosition, glm::vec3 actorSize);

	float AnimationPeriod;
	AnimationType CurrentAnimation;
	glm::vec3 Direction;
	float ChangeDirectionPeriod;
	float LastDirectionChange;
	float AnimationProgress;
	const float MAX_SPEED = 100.0f;

	std::shared_ptr<PhysicActor> AggroCollider = nullptr;

	std::shared_ptr<Actor> AggroedActor = nullptr;
};
