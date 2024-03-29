#pragma once
#include "Common.h"

//thread
class Camera;
class ParticleManager : Noncopyable
{
  private:
    uint32 _particleCount;
    uint32 _VAO;
    uint32 _textureID;
    
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