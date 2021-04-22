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
  // std::string projectSrcDir = PROJECT_SOURCE_DIR;
  // ReadFileLines(projectSrcDir + "/Aster/Textures/" + filename, m_frames);
  // m_framesCount = (int)m_frames.size();

  unsigned int word;
  std::string line;
  std::ifstream fstream(filename);
  
  if (fstream)
  {
      while (std::getline(fstream, line)) // read each line from file
      {
          std::istringstream sstream(line);
          std::vector<unsigned int> row;
          while (sstream >> word) // read each word separated by spaces
          {
              row.push_back(word);
          }
          glm::vec4 frame = glm::vec4(row[0], row[1], row[2], row[3]);
          m_frames.push_back(frame);
      }
  }

  m_framesCount = (int)m_frames.size();

  // std::cout << "m_frames = { ";
  // for (std::vector<unsigned int> n : m_frames) {
  //   for (int m : n) {
  //     std::cout << m << ", ";
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "}; \n";
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

  glm::vec4 frame = m_frames[m_spriteIndex];

  frame.x /= texture.Width;
  frame.y /= texture.Height;
  frame.z /= texture.Width;
  frame.w /= texture.Height;

  float vertices[] = {
    0.0f, 1.0f, frame.x,            frame.w,
    1.0f, 0.0f, frame.x + frame.z,  frame.y,
    0.0f, 0.0f, frame.x,            frame.y,

    0.0f, 1.0f, frame.x,            frame.w,
    1.0f, 1.0f, frame.x + frame.z,  frame.w,
    1.0f, 0.0f, frame.x + frame.z,  frame.y
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
