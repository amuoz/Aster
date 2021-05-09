#pragma once

#include "AsterTypes.h"

class RectangleSystem
{
  public:
    static RectangleSystem* GetInstance();
    Aster::Rectangle Generate();
    void Delete(Aster::Rectangle& rectangle);

  private:
    RectangleSystem();
    static RectangleSystem* m_instance;
};
