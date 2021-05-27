#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "Actor.h"
#include "AsterTypes.h"

enum class AnimationType;
class Sprite;

class PowerUp : public Actor
{
public:
	PowerUp(PowerUpType type, glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color = glm::vec3(1.0f));
	~PowerUp();

	void OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other) override;
	void OnEndOverlapFunction(std::shared_ptr<PhysicActor> other) override;

private:
	PowerUpType Type;

};
