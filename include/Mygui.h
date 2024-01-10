#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

struct GLFWwindow;
class Simulator;

class Mygui
{
  public:
    static Mygui& getInstance() {
          static Mygui instance; // Meyers' Singleton
          return instance;
    }

  public:
    void initialize(GLFWwindow* window);
    void update(Simulator& simul);
    void render(void);
    ~Mygui();
  
  private:
    Mygui(){};
};

