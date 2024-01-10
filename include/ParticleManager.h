#pragma once
#include "Common.h"
#include "Particle.h"
//thread
class Camera;
class ParticleManager : Noncopyable
{
  private:
    uint32 _particleCount;
    uint32 _VAO;

  public:
    glm::vec3 _camPos;
    uint32 _VBO;

  public:
    ParticleManager(uint32 count) : _particleCount(count){};
    ~ParticleManager();

    void initialize();
    void draw(uint32 drawCount);
    uint32 getParticleCount(){return _particleCount;};
};