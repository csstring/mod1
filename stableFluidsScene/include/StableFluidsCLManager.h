#pragma once
#include "Common.h"
#include "KernelProgram.h"
#include <random>
#include "EnumHeader.h"

enum SFKernelFunc {
  SOURCING,
  ADVECT,
  COPY,
  PRINT,
  DIVERGENCE,
  JACOBI,
  APPLYPRESSURE,
  DIFFUSE,
  COMPUTEVORTICITY,
  CONFINEVORTICITY
};

class Texture2D;
static const std::vector<glm::vec4> rainbow = {
                {1.0f, 0.0f, 0.0f, 1.0f},  // Red
                {1.0f, 0.65f, 0.0f, 1.0f}, // Orange
                {1.0f, 1.0f, 0.0f, 1.0f},  // Yellow
                {0.0f, 1.0f, 0.0f, 1.0f},  // Green
                {0.0f, 0.0f, 1.0f, 1.0f},  // Blue
                {0.3f, 0.0f, 0.5f, 1.0f},  // Indigo
                {0.5f, 0.0f, 1.0f, 1.0f}   // Violet/Purple
            };
class StableFluidsCLManager
{
  private:
    cl_platform_id platform_id;
    cl_device_id device_id;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;
    cl_command_queue command_queue;
    cl_sampler samplerLinearWrap, samplerPointWrap;
    cl_mem CLTextureID[9];

    const size_t global_work_size[2] = {static_cast<size_t>(WINDOW_WITH)/4, static_cast<size_t>(WINDOW_HEIGHT)/4};
    std::vector<KernelProgram> _programs;
    void printTextureColor(TEXTUREID);
    void copyImage(TEXTUREID src, TEXTUREID dst);

  public:
    std::random_device _rd;
    cl_context context;
    StableFluidsCLManager(){};
    ~StableFluidsCLManager();
    void initialize(Texture2D* textureids);
    void sourcing(const glm::vec4& cursor);
    void vorticity(float dt);
    void diffuse(float dt,float viscosity);
    void projection();
    void advection(float dt);
    void stableFluidsGenerate(float dt, const glm::vec4& gravity);
    void update(float dt, const glm::vec4& gravity, int32 drawCount);


    cl_mem debug1, debug2, debug3, debug4;
    float* A;
    float* B;
    float* C;
    float* D;
    const int count = WINDOW_HEIGHT * WINDOW_WITH;
};
