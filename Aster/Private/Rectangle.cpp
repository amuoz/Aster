#include "Rectangle.h"

#include "Common.h"

RectangleSystem* RectangleSystem::m_instance = nullptr;

RectangleSystem* RectangleSystem::GetInstance()
{
  if (m_instance == nullptr) m_instance = new RectangleSystem();
  return m_instance;
}

Rectangle RectangleSystem::Generate()
{
  Rectangle result;

  float vertices[] = {
    // pos      // tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
  };

  glGenVertexArrays(1, &result.vertexArrayObject);
  glGenBuffers(1, &result.vertexBufferObject);

  glBindBuffer(GL_ARRAY_BUFFER, result.vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(result.vertexArrayObject);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return result;
}

void RectangleSystem::Delete(Rectangle& rectangle)
{
  glDeleteVertexArrays(1, &rectangle.vertexArrayObject);
  glDeleteBuffers(1, &rectangle.vertexBufferObject);
  glDeleteBuffers(1, &rectangle.colorBufferObject);
  glDeleteBuffers(1, &rectangle.uvBufferObject);
}

RectangleSystem::RectangleSystem()
{
}

void Rectangle::Draw()
{
  glBindVertexArray(vertexArrayObject);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
