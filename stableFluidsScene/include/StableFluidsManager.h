#pragma once
#include "Common.h"

//thread
class Camera;

class StableFluidsManager : Noncopyable
{
  private:
    uint32 _VAO;
    
  public:
    glm::vec3 _camPos;
    uint32 _VBO;
    uint32 m_velocity, m_velocityTemp;
    uint32 m_vorticity;
    uint32 m_pressure, m_pressureTemp;
    uint32 m_divergence;
    uint32 m_density, m_densityTemp;

  public:
    StableFluidsManager(uint32 count){};
    ~StableFluidsManager();

    void initialize();
    void draw(uint32 drawCount);
};