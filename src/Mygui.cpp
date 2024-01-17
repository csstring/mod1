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

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();                       
  ImGui::Begin("Scene controller");

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::Text("carmera pos x : %f y : %f z : %f", cam._cameraPos.x, cam._cameraPos.y, cam._cameraPos.z);
  ImGui::SliderInt("Simul Speed", &simul._speed, 0, 5);

  if (ImGui::Button("particle scene"))                           
      simul._currentScene = simul._particleScene.get();
  if (ImGui::Button("stable Fluids scene"))
      simul._currentScene = simul._stableFluidsScene.get();
  if (ImGui::Button("cloudScene scene"))
      simul._currentScene = simul._cloudScene.get();

  ImGui::End();
  simul._currentScene->guiRender();
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