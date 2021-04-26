#pragma once

struct Rectangle;
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
