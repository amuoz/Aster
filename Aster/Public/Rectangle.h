#pragma once

struct Rectangle
{
  unsigned int vertexArrayObject;
  unsigned int vertexBufferObject;
  unsigned int colorBufferObject;
  unsigned int uvBufferObject;

  void Draw();
};

class RectangleSystem
{
  public:
    static RectangleSystem* GetInstance();
    Rectangle Generate();
    void Delete(Rectangle& rectangle);

  private:
    RectangleSystem();
    static RectangleSystem* m_instance;
};
