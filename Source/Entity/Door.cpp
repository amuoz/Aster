#include "Door.h"

#include "Common.h"
#include "Config.h"
#include "Sprite.h"
#include "Physics.h"
#include "PhysicActor.h"
#include <iostream>

Door::Door(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec4 color)
		: Actor(pos, size, std::move(sprite), color)
{
	ZIndex = 0.1f;
}

Door::~Door()
{
	//std::cout << "Door destroyed" << std::endl;
}
