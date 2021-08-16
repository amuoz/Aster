#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "Entity/Actor.h"
#include "Physics.h"

enum class AnimationType;
class Sprite;

class Enemy : public Actor
{
public:
	Enemy(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, float framePeriod, glm::vec4 color = glm::vec4(1.0f));
	~Enemy();

	void BeginPlay() override;
	void Update(float, glm::vec4) override;
	void Draw(SpriteRenderer &renderer, double deltaTime) override;
	void Destroy() override;

	virtual void OnBeginOverlapAggro(std::shared_ptr<PhysicActor> other) = 0;
	virtual void OnEndOverlapAggro(std::shared_ptr<PhysicActor> other) = 0;

protected:

	float StillChance;
	float ChangeDirectionChance;
	float AggroSize;

	std::shared_ptr<PhysicActor> AggroCollider = nullptr;
	std::shared_ptr<Actor> AggroedActor = nullptr;

private:

	bool PassRandomChance(float chance);
	float GetRandomDirectionComponent();
	glm::vec2 GetRandomDirection();
	glm::vec2 SetWanderMovement();
	glm::vec2 SetAggroMovement();
	void SetSpeed();
	glm::vec2 GetAggroPosition(glm::vec2 actorPosition, glm::vec3 actorSize);

	float AnimationPeriod;
	AnimationType CurrentAnimation;
	glm::vec2 Direction;
	float ChangeDirectionPeriod;
	float LastDirectionChange;
	float AnimationProgress;
	const float MAX_SPEED = 100.0f;
};
