#include "Sprite.h"

#include <string>

#include "Animation.h"
#include "Texture2D.h"
#include "RectangleSystem.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

Sprite::Sprite()
{
    HasAnimation = false;
}

Sprite::Sprite(std::string filename)
{
  Texture = ResourceManager::GetInstance()->GetTexture(filename);
  SubspriteRectangle = RectangleSystem::GetInstance()->Generate();
  HasAnimation = false;
}

Sprite::~Sprite()
{
  RectangleSystem::GetInstance()->Delete(SubspriteRectangle);

  Animations.clear();
}

void Sprite::Draw(AnimationType type, SpriteRenderer &renderer, double deltatime, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f))
{

  if (HasAnimation)
  {
    bool restartAnimation = CurrentAnimationType != type;
    CurrentAnimationType = type;
    Animations[type]->Play(renderer, Texture, SubspriteRectangle, deltatime, position, size, rotate, color, restartAnimation);
  }
  else
  {
    Texture2D texture = GetTexture();
    renderer.DrawSprite(texture, position, size, rotate, color);
  }
}

void Sprite::AddAnimation(std::string filename, AnimationType type, float speed)
{
    Animations[type] = std::make_unique<Animation>(filename, speed);
    HasAnimation = true;
}

Texture2D Sprite::GetTexture()
{
  return Texture;
}

glm::vec4 Sprite::GetAttackHitbox(AnimationType type)
{
  return Animations[type]->GetAttackHitbox();
}

int Sprite::GetFramesCount()
{
  if (HasAnimation)
  {
    return Animations[CurrentAnimationType]->GetFramesCount();
  }
  else
  {
    return 1;
  }
}

float Sprite::GetAnimationSpeed()
{
  if (HasAnimation)
  {
    return Animations[CurrentAnimationType]->GetSpeed();
  }
  else
  {
    return 0;
  }
}

AnimationType Sprite::GetAnimationType()
{
  return CurrentAnimationType;
}

float Sprite::GetAnimationLength()
{
  return GetFramesCount() * GetAnimationSpeed();
}
