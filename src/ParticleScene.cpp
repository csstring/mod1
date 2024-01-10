#include "Scene.h"
#include "Camera.h"
#include "ParticleScene.h"

void ParticleScene::initialize()
{
  _particleManager.initialize();
  _CLManager.initialize(_particleManager._VBO, _particleManager.getParticleCount());
  _CLManager.initPlane();
}

void ParticleScene::update(float dt)
{
  glm::vec4 gravity = Camera::getInstance().getWorldCursorPos();
  if (_isGravityOn == false)
    gravity.w = 0;
  if (_isGeneratorOn == true)
    _CLManager.particleGenerate(dt, gravity);
  _CLManager.update(dt, gravity, drawCount);
}

void ParticleScene::draw()
{
  _particleManager.draw(drawCount);
}