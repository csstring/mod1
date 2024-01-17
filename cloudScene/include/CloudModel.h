#pragma once
#include "Common.h"
#include "Texture3D.h"

struct Vertex
{
  glm::vec4 positions;
  glm::vec3 normal;
  glm::vec2 texcoords;
  glm::vec3 colors;
};

class CloudModel
{
  private:
    uint32 _VAO, _VBO;
    void initTexture();

  public:
    CloudModel(){};
    ~CloudModel();
    void initialize();
    void draw();

    Texture3D _textures[2];
};
