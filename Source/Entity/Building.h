#pragma once

#include <glm/glm.hpp>
#include <list>
#include "Entity/Actor.h"

class Sprite;

class Building : public Actor
{
public:
	Building(glm::vec2 doorPosition,
			glm::vec3 doorSize,
			std::unique_ptr<Sprite> doorSprite,
			std::unique_ptr<Sprite> floorSprite,
			glm::vec4 floorColor,
			std::list<std::shared_ptr<Actor> > roomActors);
	virtual ~Building();


	void BeginPlay();
	void Update(float deltaTime, glm::vec4 playerAttackHitbox);
	void Draw(SpriteRenderer &renderer, double deltatime);

	void OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other) override;
	void OnEndOverlapFunction(std::shared_ptr<PhysicActor> other) override;

private:
	std::list<std::shared_ptr<Actor> > Actors;
};
