#include "Sprite.h"

#include <string>

#include "Common.h"
#include "Animation.h"
#include "Texture2D.h"
#include "Rectangle.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

Sprite::Sprite()
{
}

Sprite::Sprite(std::string filename)
{
  m_texture = ResourceManager::GetInstance()->GetTexture(filename);
	m_rectangle = RectangleSystem::GetInstance()->Generate();
  m_hasAnimation = false;
}

Sprite::~Sprite()
{
	RectangleSystem::GetInstance()->Delete(m_rectangle);

  delete m_animation;
}

void Sprite::Draw(SpriteRenderer &renderer, double deltatime, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f))
{

	if(m_hasAnimation)
	{
    // m_animation->SetAnimationSpeed(50.0f);
		// m_animation->Play(m_texture, m_rectangle, deltatime);
    // m_texture.Bind();
  	// m_rectangle.Draw();
    renderer.DrawSubSprite(this, position, glm::vec2(100.0f, 100.0f), rotate, color);
	}
  else 
  {
    renderer.DrawSprite(this, position, size, rotate, color);
  }

  // renderer.DrawSprite(this, position, size, rotate, color);

}

void Sprite::AddAnimation(std::string filename)
{
  m_animation = new Animation(filename);
  m_hasAnimation = true;
}

Texture2D Sprite::GetTexture()
{
  return m_texture;
}