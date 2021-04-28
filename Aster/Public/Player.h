#pragma once

#include <glm/glm.hpp>
#include "Actor.h"
#include "ICircleContactReport.h"

#include "Sprite.h"

enum class AnimationType;

class Player : public Actor, ICircleContactReport
{
public:

	Player(glm::vec3 pos, glm::vec3 size, Sprite* sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	~Player();

	void Render(Shader shader);
	void Update(float, glm::vec4);
	void Draw(SpriteRenderer &renderer, double deltaTime);
	void TakeDamage();

	void Move(float deltaTime, glm::vec3 direction);
	void Idle();
	void Attack();

	void OnContact(Physics::PhysicActor* other) override;
	glm::vec4 GetAttackHitbox();

private:
	void SetState(ActorState);
	const float PLAYER_VELOCITY = 200.0f;
	ActorState State;
	ActorState LastState;
	AnimationType CurrentAnimation;
};