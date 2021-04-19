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

  glm::vec2 arr[6] = {
    glm::vec2(0,0),
    glm::vec2(0,1),
    glm::vec2(1,0),
    glm::vec2(1,0),
    glm::vec2(0,1),
    glm::vec2(1,1)
  };

  glm::vec2 colors[6] = {
    glm::vec4(1,1,1,1),
    glm::vec4(1,1,1,1),
    glm::vec4(1,1,1,1),
    glm::vec4(1,1,1,1),
    glm::vec4(1,1,1,1),
    glm::vec4(1,1,1,1)
  };

  glm::vec2 uv[6] = {
    glm::vec2(0,0),
    glm::vec2(0,1),
    glm::vec2(1,0),
    glm::vec2(1,0),
    glm::vec2(0,1),
    glm::vec2(1,1)
  };

  glGenVertexArrays(1, &result.vertexArrayObject);
  glGenBuffers(1, &result.vertexBufferObject);
  glGenBuffers(1, &result.colorBufferObject);
  glGenBuffers(1, &result.uvBufferObject);

  glBindVertexArray(result.vertexArrayObject);

  // vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, result.vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec2), &arr[0], GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, NULL);

  // color buffer
  glBindBuffer(GL_ARRAY_BUFFER, result.colorBufferObject);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec4), &colors[0], GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, NULL);

  // uv buffer
  glBindBuffer(GL_ARRAY_BUFFER, result.uvBufferObject);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec2), &uv[0], GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, NULL);


  glBindBuffer(GL_ARRAY_BUFFER, 0);

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
