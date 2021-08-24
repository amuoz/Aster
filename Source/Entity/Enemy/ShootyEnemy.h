#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "Entity/Actor.h"
#include "Entity/Enemy/Enemy.h"
#include "Physics.h"

enum class AnimationType;
class Sprite;
class Projectile;

class ShootyEnemy : public Enemy
{
public:
	ShootyEnemy(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, float framePeriod, glm::vec4 color = glm::vec4(1.0f));
	~ShootyEnemy();

	void BeginPlay() override;
	void Update(float, glm::vec4) override;
	void Draw(SpriteRenderer &renderer, double deltaTime) override;

	void OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other) override;
	void OnEndOverlapFunction(std::shared_ptr<PhysicActor> other) override;

	void OnBeginOverlapAggro(std::shared_ptr<PhysicActor> other);
	void OnEndOverlapAggro(std::shared_ptr<PhysicActor> other);

private:

	bool hasShot;
	float ShootDelay;
	float ShootInterval;
	float AttackProgress;
	float AggroProgress;
	int ArrowShiftX;
	int ArrowShiftY;
	std::unique_ptr<Projectile> Arrow;

	void UpdateAggroState(float deltaTime, glm::vec4 attackHitbox);
	void UpdateShootState(float deltaTime, glm::vec4 attackHitbox);
	void UpdateAttackState(float deltaTime, glm::vec4 attackHitbox);
	void UpdateArrow(float deltaTime, glm::vec4 attackHitbox);
	void DrawArrow(SpriteRenderer &renderer, double deltatime);
	AnimationType GetAnimationFromState() override;
	void SetSpeed() override;
	virtual glm::vec2 GetAggroDirection() override;
	void SetAttackState();
	void OnAnimationEnd();
	void CreateArrow(ActorState attackDirection);
	AnimationType GetShootAnimation();

	const float MAX_SPEED = 100.0f;
};
