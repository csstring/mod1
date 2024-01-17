#include "Scene.h"
#include "Camera.h"
#include "CloudScene.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void CloudScene::initialize()
{
  _cloudShader.initialize();
  _cloudManager.initialize();
  _cloudCLManager.initialize(_cloudManager._cloud._textures);
  _lightDir = {0.0f,1.0f,0.0f};
  _lightColor = glm::vec3(40);
  // _cloudCLManager.densityUpdate();
}

void CloudScene::update(float dt)
{
  _cloudCLManager.lightMapUpdate(_lightColor);
}

void CloudScene::guiRender()
{
  ImGui::Begin("Mandatory");                         

  ImGui::End();
}

void CloudScene::draw()
{
  Camera& camera = Camera::getInstance();
  _cloudShader.use();
  _cloudShader.setMat4("projection", camera.getProjection());
  _cloudShader.setMat4("view", camera._view);
  _cloudManager.draw();
}