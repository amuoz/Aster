#pragma once

#include <string>
#include <map>
#include <memory>

#include "Texture2D.h"
#include "RectangleSystem.h"
#include "Animation.h"

class Texture2D;
struct Rectangle;
class Animation;
class SpriteRenderer;

class Sprite
{
public:
	Sprite();
	Sprite(std::string filename);

	~Sprite();

	void Draw(AnimationType type, SpriteRenderer &renderer, double deltatime, glm::vec3 position, glm::vec2 size, float rotate, glm::vec4 color);

	void AddAnimation(std::string filename, AnimationType type, float speed);

	Texture2D GetTexture();
	glm::vec4 GetAttackHitbox(AnimationType);
	int GetFramesCount();
	float GetAnimationSpeed();
	AnimationType GetAnimationType();
	float GetAnimationLength();

private:
	Texture2D Texture;
	Aster::Rectangle SubspriteRectangle;
	bool HasAnimation;
	AnimationType CurrentAnimationType;
	std::map<AnimationType, std::unique_ptr<Animation>> Animations;
};