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

  void Play(Texture2D& sprite, Rectangle& rectangle, double deltatime);
  void SetAnimationSpeed(float speed);

private:

  double m_animationCursor;
  int m_spriteIndex;
  int m_framesCount;
  float m_speed;

  std::vector<std::vector<int> > m_frames;

};
