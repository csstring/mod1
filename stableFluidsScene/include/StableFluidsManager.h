#pragma once
#include "Common.h"
#include "Texture2D.h"
//thread
class Camera;

class StableFluidsManager : Noncopyable
{
  private:
    uint32 _VAO;
    uint32 _tmptexID;
    void initTexture();

  public:
    glm::vec3 _camPos;
    uint32 _VBO;
    Texture2D _textures[9];

  public:
    StableFluidsManager(){};
    ~StableFluidsManager();

    void initialize();
    void draw();
};