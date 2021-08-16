#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "Entity/Actor.h"
#include "Entity/Enemy/Enemy.h"
#include "Physics.h"

enum class AnimationType;
class Sprite;

class SpikeEnemy : public Enemy
{
public:
	SpikeEnemy(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, float framePeriod, glm::vec4 color = glm::vec4(1.0f));
	~SpikeEnemy();

	void BeginPlay() override;
	void Update(float, glm::vec4) override;
	void Draw(SpriteRenderer &renderer, double deltaTime) override;

	void OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other) override;
	void OnEndOverlapFunction(std::shared_ptr<PhysicActor> other) override;

	void OnBeginOverlapAggro(std::shared_ptr<PhysicActor> other);
	void OnEndOverlapAggro(std::shared_ptr<PhysicActor> other);

private:

	void SetSpeed() override;

	const float MAX_SPEED = 100.0f;
};
