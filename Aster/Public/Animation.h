#pragma once

#include <string>
#include <vector>

class Texture2D;
class Rectangle;

class Animation
{
public:

	Animation();
	Animation(std::string filename);
	virtual ~Animation();

  void Play(Texture2D& sprite, Rectangle& rectangle, double deltatime);
  void SetAnimationSpeed(float speed);

  // Texture2D GetSprite();

private:

  double m_animationCursor;
  int m_spriteIndex;
  int m_framesCount;
  float m_speed;
  std::vector<std::vector<unsigned int> > m_frames;

  // Texture2D m_spritesheet;
  // short m_spriteIndex;
  // std::vector<std::vector<unsigned int> > m_frames;

};
