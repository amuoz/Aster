#pragma once

#include <glm/glm.hpp>
#include "Actor.h"

class Texture2D;
class Sprite;

class Door : public Actor
{
public:
	Door(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec4 color = glm::vec4(1.0f));
	virtual ~Door();

private:
};
