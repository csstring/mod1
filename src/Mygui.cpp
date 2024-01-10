#include "Mygui.h"
#include "Camera.h"
#include "Simulator.h"
#include "Scene.h"
#include "ParticleScene.h"

void Mygui::initialize(GLFWwindow* window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
}

void Mygui::update(Simulator& simul)
{
  auto& cam = Camera::getInstance();
  ParticleScene* ps = static_cast<ParticleScene*>(simul._particleScene.get());

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Mandatory");                         

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::Text("carmera pos x : %f y : %f z : %f", cam._cameraPos.x, cam._cameraPos.y, cam._cameraPos.z);

  if (ImGui::Button("Circle Shader"))                           
      ps->particleShape = PARTICLE_SHAPE::CIRCLE;
  if (ImGui::Button("Qurd Shader"))
      ps->particleShape = PARTICLE_SHAPE::QURD;
  
  ImGui::SliderInt("Particle Count", &ps->drawCount, 0, ps->_particleManager.getParticleCount());
  ImGui::End();

  ImGui::Begin("Bonus"); 

  ImGui::SliderInt("Particle Size", &ps->_pointSize, 1, 10);
  ImGui::SliderInt("Simul Speed", &simul._speed, 0, 5);
  ImGui::Checkbox("Enable Generator", &ps->_isGeneratorOn);

  if (ImGui::Button("init Plane scene"))                           
      ps->_ParticleCLManager.initPlane();
  if (ImGui::Button("init circle scene"))
      ps->_ParticleCLManager.initCircle();

  ImGui::End();
}

void Mygui::render(void)
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Mygui::~Mygui()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}