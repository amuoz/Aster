#pragma once

#include <glm/glm.hpp>
#include "Actor.h"
#include "AsterTypes.h"

class Texture2D;
class Sprite;

class Block : public Actor
{
public:

	Block(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color = glm::vec3(1.0f), BlockLocation location = BlockLocation::MIDDLE);
	virtual ~Block();

	void Update(float deltaTime, glm::vec4 playerAttackHitbox) override;

private:
	float SizeRatioX;
	float SizeRatioY;

	BlockLocation Location;

	glm::vec2 GetPhysicsPosition(glm::vec2 pos, glm::vec3 size, BlockLocation location);
	glm::vec3 GetPhysicsSize(glm::vec3 size, BlockLocation location);
	bool IsTopLocation();
};
