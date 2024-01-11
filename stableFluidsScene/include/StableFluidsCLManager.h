#pragma once
#include "Common.h"
#include "KernelProgram.h"
#include <random>

enum KernelFunc {
  MAIN_ROOP,
  INIT_CIRCLE,
  INIT_PLANE,
  GENERATOR
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

    cl_mem m_velocity, m_velocityTemp;
    cl_mem m_vorticity;
    cl_mem m_pressure, m_pressureTemp;
    cl_mem m_divergence;
    cl_mem m_density, m_densityTemp;

    uint64 global_item_size = 1024;
    std::vector<KernelProgram> _programs;

  public:
    std::random_device _rd;
    cl_context context;
    stableFluidsCLManager(){};
    ~stableFluidsCLManager();
    void initialize(uint32 VBO, uint64 count);
    void initCircle();
    void initPlane();
    void stableFluidsGenerate(float dt, const glm::vec4& gravity);
    void update(float dt, const glm::vec4& gravity, int32 drawCount);
};
