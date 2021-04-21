#pragma once

#include <glm/glm.hpp>
#include "Actor.h"
#include "ICircleContactReport.h"

class Texture2D;

class Block : public Actor, ICircleContactReport
{
public:

	Block(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~Block();

	virtual void Render(Shader shader);
	virtual void Update(float deltaTime);

	void OnContact(Physics::PhysicActor* other) override;
};