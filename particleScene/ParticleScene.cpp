#include "Scene.h"
#include "Camera.h"
#include "ParticleScene.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void ParticleScene::initialize()
{
  _particleShader.initialize();
  _particleManager.initialize();
  _ParticleCLManager.initialize(_particleManager._VBO, _particleManager.getParticleCount());
  _ParticleCLManager.initPlane();
}

void ParticleScene::update(float dt)
{
  glm::vec4 gravity = Camera::getInstance().getWorldCursorPos();
  if (_isGravityOn == false)
    gravity.w = 0;
  if (_isGeneratorOn == true)
    _ParticleCLManager.particleGenerate(dt, gravity);
  _ParticleCLManager.update(dt, gravity, drawCount);
}

void ParticleScene::guiRender()
{
  auto& cam = Camera::getInstance();

  ImGui::Begin("Mandatory");                         

  if (ImGui::Button("Circle Shader"))                           
      particleShape = PARTICLE_SHAPE::CIRCLE;
  if (ImGui::Button("Qurd Shader"))
      particleShape = PARTICLE_SHAPE::QURD;
  
  ImGui::SliderInt("Particle Count", &drawCount, 0, _particleManager.getParticleCount());
  ImGui::End();

  ImGui::Begin("Bonus"); 

  ImGui::SliderInt("Particle Size", &_pointSize, 1, 30);
  ImGui::Checkbox("Enable Generator", &_isGeneratorOn);

  if (ImGui::Button("init Plane scene"))                           
      _ParticleCLManager.initPlane();
  if (ImGui::Button("init circle scene"))
      _ParticleCLManager.initCircle();

  ImGui::End();
}

void ParticleScene::draw()
{
  Camera& camera = Camera::getInstance();
  _particleShader.use();
  _particleShader.setMat4("projection", camera.getProjection());
  _particleShader.setMat4("view", camera._view);
  _particleShader.setVec4("cursorPos", camera.getProjection() * camera._view * camera.getWorldCursorPos());
  glPointSize(_pointSize);
  switch (particleShape)
  {
    case PARTICLE_SHAPE::CIRCLE:
      _particleShader.setUint("shape", 1);
      break;
    case PARTICLE_SHAPE::QURD:
      _particleShader.setUint("shape", 0);
      break;
  }
  _particleManager.draw(drawCount);
}