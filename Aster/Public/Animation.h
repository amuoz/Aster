#pragma once

#include <string>
#include <vector>

#include "Common.h"

class Texture2D;
class Rectangle;
class SpriteRenderer;

enum class AnimationType {
    IDLE,
    ATTACK_UP,
    ATTACK_RIGHT,
    ATTACK_DOWN,
    ATTACK_LEFT,
};
 
class Animation
{
public:

	Animation();
	Animation(std::string filename, float speed);
	virtual ~Animation();

  void Play(SpriteRenderer &renderer, Texture2D& texture, Rectangle& rectangle, double deltatime, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);
  void SetAnimationSpeed(float speed);

private:

  double m_animationCursor;
  int m_spriteIndex;
  int m_framesCount;
  float m_speed;

  std::vector<std::vector<int> > m_frames;

};
