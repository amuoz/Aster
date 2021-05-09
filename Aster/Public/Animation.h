#pragma once

#include <string>
#include <vector>

#include "Common.h"

class Texture2D;
struct Rectangle;
class SpriteRenderer;

class Animation
{
public:
  Animation();
  Animation(std::string filename, float speed);
  virtual ~Animation();

  void Play(
      SpriteRenderer &renderer,
      Texture2D &texture,
      Aster::Rectangle &rectangle,
      double deltatime,
      glm::vec2 position,
      glm::vec2 size,
      float rotate,
      glm::vec3 color,
      bool restartAnimation);

  glm::vec4 GetAttackHitbox();

private:
  double m_animationCursor;
  int m_spriteIndex;
  int FramesCount;
  float m_speed;

  std::vector<std::vector<int> > Frames;
  std::vector<std::vector<int> > HitboxFrames;
};
