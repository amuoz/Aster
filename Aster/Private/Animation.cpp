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
  

  // std::vector<unsigned int> frame = m_frames[m_spriteIndex];

  // // frame[0] /= texture.Width;
  // // frame[1] /= texture.Height;
  // // frame[2] /= texture.Width;
  // // frame[3] /= texture.Height;

  // std::vector<glm::vec2> uv;

  // uv = {
  //   glm::vec2(frame[0], frame[1]),
  //   glm::vec2(frame[0], frame[1] + frame[3]),
  //   glm::vec2(frame[0], frame[2] + frame[1]),

  //   glm::vec2(frame[0] + frame[2], frame[1]),
  //   glm::vec2(frame[0], frame[1] + frame[3]),
  //   glm::vec2(frame[0] + frame[2], frame[1] + frame[3])
  // };

  glm::vec4 frame = m_frames[m_spriteIndex];

  frame.x /= texture.Width;
  frame.y /= texture.Height;
  frame.z /= texture.Width;
  frame.w /= texture.Height;



  std::vector<glm::vec2> uv;

  uv = {
    glm::vec2(frame.x, frame.y),
    glm::vec2(frame.x, frame.y + frame.w),
    glm::vec2(frame.x, frame.z + frame.y),

    glm::vec2(frame.x + frame.z, frame.y),
    glm::vec2(frame.x, frame.y + frame.w),
    glm::vec2(frame.x + frame.z, frame.y + frame.w)
  };

  glBindVertexArray(rectangle.vertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, rectangle.uvBufferObject);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec2), &uv[0], GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, NULL);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);



  // glBindVertexArray(rectangle.vertexArrayObject);
  // glBindBuffer(GL_ARRAY_BUFFER, rectangle.uvBufferObject);

  // void* gpubuffer = nullptr;
  // gpubuffer = glMapBufferRange(GL_ARRAY_BUFFER, 0, 6 * sizeof(glm::vec2), GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT);
  // memcpy(gpubuffer, uv.data(), 6 * sizeof(glm::vec2));
  // glUnmapBuffer(GL_ARRAY_BUFFER);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Animation::SetAnimationSpeed(float speed)
{
  m_speed = speed;
}

// Texture2D Animation::GetSprite()
// {
  
// 	return ResourceManager::GetInstance()->CropTextureFromFile(PROJECT_SOURCE_DIR "/Aster/Textures/player_walk.png", true);
// }
