#pragma once

#include <glm/glm.hpp>
#include "Actor.h"
#include "ICircleContactReport.h"

#include "Sprite.h"

enum class AnimationType;

class SpikeEnemy : public Actor, ICircleContactReport
{
public:
	SpikeEnemy(glm::vec3 pos, glm::vec3 size, Sprite* sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	~SpikeEnemy();

	void Render(Shader shader);
	void Update(float, glm::vec4);
	void Draw(SpriteRenderer &renderer, double deltaTime);

	void OnContact(Physics::PhysicActor* other) override;

private:
	void SetState(ActorState);

	ActorState State;
	ActorState LastState;
	AnimationType CurrentAnimation;
};
