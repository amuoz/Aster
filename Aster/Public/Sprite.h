#pragma once

#include <string>

#include "Common.h"
#include "Texture2D.h"
#include "Rectangle.h"

class Animation;
class SpriteRenderer;

class Sprite
{
public:
	Sprite();
	Sprite(std::string filename);

	~Sprite();

	void Draw(SpriteRenderer &renderer, double deltatime, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);

	void AddAnimation(std::string filename);
	void SetAnimationSpeed(float speed);

  Texture2D GetTexture();

private:
	Texture2D m_texture;
	Rectangle m_rectangle;
  bool m_hasAnimation;

	Animation* m_animation;
};