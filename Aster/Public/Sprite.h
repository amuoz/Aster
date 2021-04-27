#pragma once

#include <string>
#include <map>

#include "Common.h"
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

	void Draw(AnimationType type, SpriteRenderer &renderer, double deltatime, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);

	void AddAnimation(std::string filename, AnimationType type, float speed);

  Texture2D GetTexture();

private:
	Texture2D m_texture;
	Rectangle m_rectangle;
  bool m_hasAnimation;

	std::map<AnimationType, Animation*> m_animations;
};