#include "StableFluidsManager.h"
#include "Camera.h"
#include "DDSLoder.h" 
#include "Fluid.h"
#include "EnumHeader.h"
#include "DDSLoder.h"

StableFluidsManager::~StableFluidsManager()
{
  glDeleteVertexArrays(1, &_VAO);
  glDeleteBuffers(1, &_VBO);
}

void StableFluidsManager::initTexture()
{
  uint64 w = static_cast<uint64>(WINDOW_WITH)/ 4;
  uint64 h = static_cast<uint64>(WINDOW_HEIGHT)/4;

  _textures[TEXTUREID::VELOCITYID].initialize(GL_RGBA32F,w,h);
  _textures[TEXTUREID::VELOCITYTMPID].initialize(GL_RGBA32F,w,h);
  _textures[TEXTUREID::VORTICITYID].initialize(GL_RGBA32F,w,h);
  _textures[TEXTUREID::PRESSUREID].initialize(GL_RGBA32F,w,h);
  _textures[TEXTUREID::PRESSURETMPID].initialize(GL_RGBA32F,w,h);
  _textures[TEXTUREID::DIVERGENCEID].initialize(GL_RGBA32F,w,h);
  _textures[TEXTUREID::DIVERGENCETMPID].initialize(GL_RGBA32F,w,h);
  _textures[TEXTUREID::DENSITYID].initialize(GL_RGBA32F,w,h);
  _textures[TEXTUREID::DENSITYTMPID].initialize(GL_RGBA32F,w,h);
}

void StableFluidsManager::initialize()
{
  std::vector<Fluid> fluids;
  fluids.push_back({glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}); // Top-left
  fluids.push_back({glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}); // Bottom-left
  fluids.push_back({glm::vec4( 1.0f, 1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}); // Bottom-right

  fluids.push_back({glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}); // Top-left
  fluids.push_back({glm::vec4( 1.0f, 1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}); // Bottom-right
  fluids.push_back({glm::vec4( 1.0f, -1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}); // Top-right

  initTexture();
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, fluids.size() * sizeof(Fluid), fluids.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Fluid), (void*)offsetof(Fluid, _position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Fluid), (void*)offsetof(Fluid, _uv));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void StableFluidsManager::draw()
{
  glBindVertexArray(_VAO);
  glBindTexture(GL_TEXTURE_2D, _textures[TEXTUREID::DENSITYID].getID());
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}
