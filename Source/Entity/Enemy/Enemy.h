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
	float AnimationPeriod;
	float AnimationProgress;
	float ChangeDirectionPeriod;
	float LastDirectionChange;
	glm::vec2 Direction;
	AnimationType CurrentAnimation;

	std::shared_ptr<PhysicActor> AggroCollider = nullptr;
	std::shared_ptr<Actor> AggroedActor = nullptr;

	virtual glm::vec2 GetWanderDirection();
	virtual glm::vec2 GetAggroDirection();
	virtual void SetSpeed() = 0;
	virtual AnimationType GetAnimationFromState();
	virtual void OnAnimationEnd();
	glm::vec4 GetColorFromState();
	glm::vec2 GetAggroPosition(glm::vec2 actorPosition, glm::vec3 actorSize);

private:

	bool PassRandomChance(float chance);
	float GetRandomDirectionComponent();
	glm::vec2 GetRandomDirection();

	const float MAX_SPEED = 100.0f;
};
