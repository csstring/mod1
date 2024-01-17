#include "CloudManager.h"
#include "Camera.h"
#include "EnumHeader.h"


CloudManager::~CloudManager()
{
}

void CloudManager::initTexture()
{
  // uint64 w = static_cast<uint64>(WINDOW_WITH)/ 4;
  // uint64 h = static_cast<uint64>(WINDOW_HEIGHT)/4;

  // _textures[TEXTUREID::VELOCITYID].initialize(GL_RGBA32F,w,h);
  // _textures[TEXTUREID::VELOCITYTMPID].initialize(GL_RGBA32F,w,h);
  // _textures[TEXTUREID::VORTICITYID].initialize(GL_RGBA32F,w,h);
  // _textures[TEXTUREID::PRESSUREID].initialize(GL_RGBA32F,w,h);
  // _textures[TEXTUREID::PRESSURETMPID].initialize(GL_RGBA32F,w,h);
  // _textures[TEXTUREID::DIVERGENCEID].initialize(GL_RGBA32F,w,h);
  // _textures[TEXTUREID::DIVERGENCETMPID].initialize(GL_RGBA32F,w,h);
  // _textures[TEXTUREID::DENSITYID].initialize(GL_RGBA32F,w,h);
  // _textures[TEXTUREID::DENSITYTMPID].initialize(GL_RGBA32F,w,h);
}

void CloudManager::initialize()
{
  _cloud.initialize();
}

void CloudManager::draw()
{
  _cloud.draw();
}
