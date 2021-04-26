#include "Animation.h"

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>

#include "stb_image.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Files.h"
#include "Rectangle.h"
#include "Common.h"

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
}

Animation::~Animation()
{

}

void Animation::Play(Texture2D& texture, Rectangle& rectangle, double deltatime)
{
  m_animationCursor += deltatime;

  if (m_animationCursor > m_speed)
  {
    m_spriteIndex = (m_spriteIndex + 1) % m_framesCount;
    m_animationCursor = 0;
  }

  std::vector<int> frame = m_frames[m_spriteIndex];

  float xPos = (float)frame[0] / texture.Width;
  float yPos = (float)frame[1] / texture.Height;
  float width = (float)frame[2] / texture.Width;
  float height = (float)frame[3] / texture.Height;

  float vertices[] = {
    0.0f, 1.0f, xPos,          height,
    1.0f, 0.0f, xPos + width,  yPos,
    0.0f, 0.0f, xPos,          yPos,

    0.0f, 1.0f, xPos,          height,
    1.0f, 1.0f, xPos + width,  height,
    1.0f, 0.0f, xPos + width,  yPos
  };

  glActiveTexture(GL_TEXTURE0);
  texture.Bind();

  glBindVertexArray(rectangle.vertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, rectangle.vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  rectangle.Draw();
}

void Animation::SetAnimationSpeed(float speed)
{
  m_speed = speed;
}
