#include "Animation.h"

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "stb_image.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Files.h"
#include "Rectangle.h"

Animation::Animation()
{
  m_spriteIndex = 0;
}

Animation::Animation(std::string filename)
  : m_animationCursor(0),
  m_spriteIndex(0),
  m_speed(0.05f)
{
  std::string projectSrcDir = PROJECT_SOURCE_DIR;
  ReadFileLines(projectSrcDir + "/Aster/Textures/" + filename, m_frames);
  m_framesCount = (int)m_frames.size();
  std::cout << "m_frames = { ";
  for (std::vector<unsigned int> n : m_frames) {
    for (int m : n) {
      std::cout << m << ", ";
    }
    std::cout << "\n";
  }
  std::cout << "}; \n";
  // std::cout << &m_frames[0][0] << std::endl;
  // std::cout << &m_frames[0][1] << std::endl;
  // std::cout << &m_frames[1][0] << std::endl;

  // m_spritesheet = ResourceManager::GetInstance()->GetTexture(name);
  // // ReadFileLines(PROJECT_SOURCE_DIR std::format("/Aster/Textures/{0}.txt", name), m_frames);
  // ReadFileLines(PROJECT_SOURCE_DIR "/Aster/Textures/player_walk.txt", m_frames);
  // m_spriteIndex = 0;
}

Animation::~Animation()
{

}

void Animation::Play(Texture2D& sprite, Rectangle& rectangle, double deltatime)
{
  m_animationCursor += deltatime;

  if (m_animationCursor > m_speed)
  {
    m_spriteIndex = (m_spriteIndex + 1) % m_framesCount;
    m_animationCursor = 0;
  }

  std::vector<unsigned int> frame = m_frames[m_spriteIndex];

  frame[0] /= sprite.Width;
  frame[1] /= sprite.Height;
  frame[2] /= sprite.Width;
  frame[3] /= sprite.Height;

  std::vector<glm::vec2> uv;

  uv = {
    glm::vec2(frame[0], frame[1]),
    glm::vec2(frame[0], frame[1] + frame[3]),
    glm::vec2(frame[0], frame[2] + frame[1]),

    glm::vec2(frame[0] + frame[2], frame[1]),
    glm::vec2(frame[0], frame[1] + frame[3]),
    glm::vec2(frame[0] + frame[2], frame[1] + frame[3])
  };

  glBindVertexArray(rectangle.vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, rectangle.uvBufferObject);

  void* gpubuffer = nullptr;
  gpubuffer = glMapBufferRange(GL_ARRAY_BUFFER, 0, 6 * sizeof(glm::vec2), GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT);
  memcpy(gpubuffer, uv.data(), 6 * sizeof(glm::vec2));
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Animation::SetAnimationSpeed(float speed)
{
  m_speed = speed;
}

// Texture2D Animation::GetSprite()
// {
  
// 	return ResourceManager::GetInstance()->CropTextureFromFile(PROJECT_SOURCE_DIR "/Aster/Textures/player_walk.png", true);
// }
