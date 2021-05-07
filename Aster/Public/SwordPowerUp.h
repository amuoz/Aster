#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "Actor.h"
#include "Sprite.h"
#include "Physics.h"

enum class AnimationType;

class SwordPowerUp : public Actor
{
public:
	SwordPowerUp(glm::vec3 pos, glm::vec3 size, Sprite* sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	~SwordPowerUp();

	void Render(Shader shader) override;
	void Update(float, glm::vec4) override;
	void TakeDamage() override;
	void OnContact(
			std::shared_ptr<Physics::PhysicActor> external,
			std::shared_ptr<Physics::PhysicActor> internal) override;

private:
};
