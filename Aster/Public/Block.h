#pragma once

#include <glm/glm.hpp>
#include "Actor.h"
#include "ICircleContactReport.h"

class Texture2D;
class Sprite;

class Block : public Actor, ICircleContactReport
{
public:

	Block(glm::vec3 pos, glm::vec3 size, Sprite* sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~Block();

	void Render(Shader shader);
	void Update(float deltaTime, glm::vec4 playerAttackHitbox);

	void OnContact(Physics::PhysicActor* other) override;

private:
};