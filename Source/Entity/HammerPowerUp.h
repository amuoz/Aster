#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "Actor.h"
#include "Physics.h"

enum class AnimationType;
class Sprite;
class PhysicActor;

class HammerPowerUp : public Actor
{
public:
	HammerPowerUp(glm::vec3 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
	~HammerPowerUp();

	void OnContact(
			std::shared_ptr<PhysicActor> external,
			std::shared_ptr<PhysicActor> internal) override;
};
