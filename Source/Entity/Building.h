#pragma once

#include <glm/glm.hpp>
#include <list>
#include "Entity/Actor.h"

class Sprite;
class PhysicActor;

class Building : public Actor
{
public:
	Building(glm::vec2 doorPosition,
			glm::vec3 doorSize,
			std::unique_ptr<Sprite> doorSprite,
			std::unique_ptr<Sprite> floorSprite,
			glm::vec4 floorColor,
			std::list<std::shared_ptr<Actor> > exteriorActors,
			std::list<std::shared_ptr<Actor> > interiorActors);
	virtual ~Building();


	void BeginPlay();
	void Update(float deltaTime, glm::vec4 playerAttackHitbox);
	void Draw(SpriteRenderer &renderer, double deltatime);

	void OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other) override;
	void OnEndOverlapFunction(std::shared_ptr<PhysicActor> other) override;

private:
	float POSITION_OFFSET = 10.0f;
	float FULL_ALPHA = 1.0f;
	float CEILING_ALPHA = 0.1f;

	std::list<std::shared_ptr<Actor> > ExteriorActors;
	std::list<std::shared_ptr<Actor> > InteriorActors;

	std::shared_ptr<PhysicActor> EntryCollider;

	bool ShowInterior;
	bool IsEntering;
	bool hasEntererd;

	glm::vec2 GetEntryPosition(glm::vec2 doorPosition);
	glm::vec3 GetEntrySize(glm::vec3 doorSize);

	void OnBeginOverlapEntry(std::shared_ptr<PhysicActor> other);
	void OnEndOverlapEntry(std::shared_ptr<PhysicActor> other);
};
