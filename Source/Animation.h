#pragma once

#include <string>
#include <vector>

#include "AsterTypes.h"
#include <glm/glm.hpp>

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
      glm::vec3 position,
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
