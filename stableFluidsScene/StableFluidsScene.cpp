#include "Scene.h"
#include "Camera.h"
#include "StableFluidsScene.h"

void StableFluidsScene::initialize()
{
  _stableFluidsShader.initialize();
  _stableFluidsManager.initialize();
  _stableFluidsCLManager.initialize(_stableFluidsManager._textures);

}

void StableFluidsScene::update(float dt)
{
  glm::vec4 gravity = Camera::getInstance().getWorldCursorPos();
  _stableFluidsCLManager.sourcing(gravity);
  _stableFluidsCLManager.vorticity(dt);
  _stableFluidsCLManager.diffuse(dt, 1.0f);
  _stableFluidsCLManager.projection();
  _stableFluidsCLManager.advection(dt);
}

void StableFluidsScene::draw()
{
  Camera& camera = Camera::getInstance();
  _stableFluidsShader.use();
  _stableFluidsShader.setMat4("projection", camera.getProjection());
  _stableFluidsShader.setMat4("view", camera._view);

  _stableFluidsManager.draw();
}