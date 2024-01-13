#include "StableFluidsManager.h"
#include "Camera.h"
#include "Particle.h"
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
  _textures[TEXTUREID::VELOCITY].initialize(GL_RGBA32F);
  _textures[TEXTUREID::VELOCITYTMP].initialize(GL_RGBA32F);
  _textures[TEXTUREID::VORTICITY].initialize(GL_RGBA32F);
  _textures[TEXTUREID::PRESSURE].initialize(GL_RGBA32F);
  _textures[TEXTUREID::PRESSURETMP].initialize(GL_RGBA32F);
  _textures[TEXTUREID::DIVERGENCE].initialize(GL_RGBA32F);
  _textures[TEXTUREID::DENSITY].initialize(GL_RGBA32F);
  _textures[TEXTUREID::DENSITYTMP].initialize(GL_RGBA32F);
}

void StableFluidsManager::initialize()
{
  std::vector<Fluid> fluids;
  fluids.push_back({glm::vec4(-1.0f,  1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}); // Top-left
  fluids.push_back({glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}); // Bottom-left
  fluids.push_back({glm::vec4( 1.0f, -1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}); // Bottom-right

  fluids.push_back({glm::vec4(-1.0f,  1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f)}); // Top-left
  fluids.push_back({glm::vec4( 1.0f, -1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}); // Bottom-right
  fluids.push_back({glm::vec4( 1.0f,  1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}); // Top-right
  DDSLoder textureLoder(std::filesystem::canonical("./stableFluidsScene/uvmap.dds"));
  _tmptexID = textureLoder.loadDDS();
  
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
  glBindTexture(GL_TEXTURE_2D, _textures[TEXTUREID::DENSITY].getID());
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}
