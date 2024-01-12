#pragma once
#include "Common.h"
#include "KernelProgram.h"
#include <random>

enum SFKernelFunc {
  SOURCING,
  ADVECT
};

class Texture2D;

class StableFluidsCLManager
{
  private:
    cl_platform_id platform_id;
    cl_device_id device_id;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;
    cl_command_queue command_queue;
    cl_sampler samplerLinearWrap;
    cl_mem CLTextureID[8];

    std::vector<KernelProgram> _programs;

  public:
    std::random_device _rd;
    cl_context context;
    StableFluidsCLManager(){};
    ~StableFluidsCLManager();
    void initialize(Texture2D* textureids);
    void sourcing(const glm::vec4& cursor);
    void stableFluidsGenerate(float dt, const glm::vec4& gravity);
    void update(float dt, const glm::vec4& gravity, int32 drawCount);
};
