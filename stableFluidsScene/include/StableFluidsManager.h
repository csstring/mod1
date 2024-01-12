#pragma once
#include "Common.h"
#include "Texture2D.h"
//thread
class Camera;

class StableFluidsManager : Noncopyable
{
  private:
    uint32 _VAO;
    void initTexture();

  public:
    glm::vec3 _camPos;
    uint32 _VBO;
    Texture2D _textures[8];

  public:
    StableFluidsManager(){};
    ~StableFluidsManager();

    void initialize();
    void draw();
};