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
#include "SpriteRenderer.h"

Animation::Animation()
{
    SpriteIndex = 0;
    m_animationCursor = 0.0f;
    Speed = 0.0f;
    FramesCount = 0;
}

Animation::Animation(std::string filename, float speed)
    : m_animationCursor(0),
      SpriteIndex(0),
      Speed(0.05f)
{
  std::string projectSrcDir = PROJECT_SOURCE_DIR;

  ReadFileLines(projectSrcDir + "/Textures/" + filename + ".anim", Frames);
  FramesCount = (int)Frames.size();

  ReadFileLines(projectSrcDir + "/Textures/" + filename + ".hit", HitboxFrames);

  Speed = speed;
}

Animation::~Animation()
{
}

void Animation::Play(SpriteRenderer &renderer,
                     Texture2D &texture,
                     Aster::Rectangle &rectangle,
                     double deltatime,
                     glm::vec3 position,
                     glm::vec2 size,
                     float rotate,
                     glm::vec4 color,
                     bool restartAnimation)
{
  if (restartAnimation)
  {
    SpriteIndex = 0;
  }

  m_animationCursor += deltatime;

  if (m_animationCursor > Speed)
  {
    SpriteIndex = (SpriteIndex + 1) % FramesCount;
    m_animationCursor = 0;
  }

  std::vector<int> frame = Frames[SpriteIndex];

  const int CELL_WIDTH = 128;
  const int CELL_HEIGHT = 128;

  // Sprite location in the spritesheet
  float xSpritePos = (float)frame[0] / texture.Width * CELL_WIDTH;
  float ySpritePos = (float)frame[1] / texture.Height * CELL_HEIGHT;
  float spriteWidth = (float)frame[2] / texture.Width * CELL_WIDTH;
  float spriteHeight = (float)frame[3] / texture.Height * CELL_HEIGHT;
  float xCenter = (float)frame[4] / 2 * CELL_WIDTH;
  float yCenter = (float)frame[5] / 2 * CELL_HEIGHT;
  // Number of Cells that the sprite occupies
  float xCells = (float)frame[2];
  float yCells = (float)frame[3];

  float vertices[] = {
      0.0f, yCells, xSpritePos, ySpritePos + spriteHeight, //  3  2   //  2  3
      xCells, 0.0f, xSpritePos + spriteWidth, ySpritePos,  //         //
      0.0f, 0.0f, xSpritePos, ySpritePos,                  //  1      //     1

      0.0f, yCells, xSpritePos, ySpritePos + spriteHeight,                 //     3   //  3
      xCells, yCells, xSpritePos + spriteWidth, ySpritePos + spriteHeight, //         //
      xCells, 0.0f, xSpritePos + spriteWidth, ySpritePos                   //  1  2   //  2  1
  };

  renderer.SetShader(glm::vec3(position.x - xCenter, position.y - yCenter, position.z), size, rotate, color);

  glActiveTexture(GL_TEXTURE0);
  texture.Bind();

  glBindVertexArray(rectangle.vertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, rectangle.vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  rectangle.Draw();
}

glm::vec4 Animation::GetAttackHitbox()
{
  std::vector<int> hitbox = HitboxFrames[SpriteIndex];
  return glm::vec4(hitbox[0], hitbox[1], hitbox[2], hitbox[3]);
}

int Animation::GetFramesCount()
{
  return FramesCount;
}

float Animation::GetSpeed()
{
  return Speed;
}
