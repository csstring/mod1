#include "Scene.h"
#include "Camera.h"
#include "StableFluidsScene.h"

void StableFluidsScene::initialize()
{
  _stableFluidsShader.initialize();
  _stableFluidsManager.initialize();
  _stableFluidsCLManager.initialize(_stableFluidsManager._textures);
  // _ParticleCLManager.initPlane();
}

void StableFluidsScene::update(float dt)
{
  glm::vec4 gravity = Camera::getInstance().getWorldCursorPos();
  _stableFluidsCLManager.sourcing(gravity);
}

void StableFluidsScene::draw()
{
  Camera& camera = Camera::getInstance();
  _stableFluidsShader.use();
  _stableFluidsShader.setMat4("projection", camera.getProjection());
  _stableFluidsShader.setMat4("view", camera._view);
  // _particleShader.setVec4("cursorPos", camera.getProjection() * camera._view * camera.getWorldCursorPos());
  // glPointSize(_pointSize);
  // switch (particleShape)
  // {
  //   case PARTICLE_SHAPE::CIRCLE:
  //     _particleShader.setUint("shape", 1);
  //     break;
  //   case PARTICLE_SHAPE::QURD:
  //     _particleShader.setUint("shape", 0);
  //     break;
  // }
  _stableFluidsManager.draw();
}