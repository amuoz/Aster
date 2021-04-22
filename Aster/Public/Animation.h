#pragma once

#include <string>
#include <vector>

#include "Common.h"

class Texture2D;
class Rectangle;
class SpriteRenderer;
 
class Animation
{
public:

	Animation();
	Animation(std::string filename);
	virtual ~Animation();

  void Play(Texture2D& sprite, Rectangle& rectangle, double deltatime);
  void SetAnimationSpeed(float speed);

private:

  double m_animationCursor;
  int m_spriteIndex;
  int m_framesCount;
  float m_speed;
  std::vector<glm::vec4> m_frames;

  // Texture2D m_spritesheet;
  // short m_spriteIndex;
  // std::vector<std::vector<unsigned int> > m_frames;

};
