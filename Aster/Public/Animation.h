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
      Rectangle &rectangle,
      double deltatime,
      glm::vec2 position,
      glm::vec2 size,
      float rotate,
      glm::vec3 color,
      bool restartAnimation);

  glm::vec4 GetAttackHitbox();
  int GetFramesCount();
  float GetSpeed();

private:
  double m_animationCursor;
  int SpriteIndex;
  int FramesCount;
  float Speed;

  std::vector<std::vector<int> > Frames;
  std::vector<std::vector<int> > HitboxFrames;
};
