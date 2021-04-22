#pragma once

#include <glm/glm.hpp>
#include "Actor.h"
#include "ICircleContactReport.h"

#include "Sprite.h"

class Player : public Actor, ICircleContactReport
{
public:

	Player(glm::vec3 pos, glm::vec3 size, Sprite* sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~Player();

	virtual void Render(Shader shader);
	virtual void Update(float deltaTime);

	void Move(float deltaTime, glm::vec3 direction);

	void OnContact(Physics::PhysicActor* other) override;

private:
	const float PLAYER_VELOCITY = 200.0f;
};