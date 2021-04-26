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
#include "RectangleSystem.h"
#include "Common.h"
#include "SpriteRenderer.h"

Animation::Animation()
{
  m_spriteIndex = 0;
}

Animation::Animation(std::string filename, float speed)
  : m_animationCursor(0),
  m_spriteIndex(0),
  m_speed(0.05f)
{
  std::string projectSrcDir = PROJECT_SOURCE_DIR;
  ReadFileLines(projectSrcDir + "/Aster/Textures/" + filename + ".txt", m_frames);
  m_framesCount = (int)m_frames.size();
  m_speed = speed;
}

Animation::~Animation()
{

}

void Animation::Play(SpriteRenderer &renderer, Texture2D& texture, Rectangle& rectangle, double deltatime, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
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
  float xCells = (float)frame[2] / 128;
  float yCells = (float)frame[3] / 128;
  int xExtend = frame[2] < 0 ? 64 : 0;
  int yExtend = frame[3] < 0 ? 64 : 0;

  float vertices[] = {
    0.0f,    yCells,   xPos,          yPos + height,   //  3  2   //  2  3
    xCells,  0.0f,     xPos + width,  yPos,            //         //
    0.0f,    0.0f,     xPos,          yPos,            //  1      //     1

    0.0f,    yCells,   xPos,          yPos + height,   //     3   //  3
    xCells,  yCells,   xPos + width,  yPos + height,   //         //
    xCells,  0.0f,     xPos + width,  yPos             //  1  2   //  2  1
  };

  renderer.SetShader(glm::vec2(position.x + xExtend, position.y + yExtend), size, rotate, color);

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
