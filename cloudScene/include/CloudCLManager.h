#pragma once
#include "Common.h"
#include "KernelProgram.h"
#include <random>
#include "EnumHeader.h"

enum CLOUDKernelFunc {
  CLOUDLIGHTING,
  CLOUDDENSITY
};

class Texture3D;

class CloudCLManager
{
  private:
    cl_platform_id platform_id;
    cl_device_id device_id;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;
    cl_command_queue command_queue;
    cl_sampler samplerLinearWrap, samplerPointWrap;
    cl_mem CLTextureID[2];

    size_t global_work_size[3] = {128,128,128};
    std::vector<KernelProgram> _programs;

  public:
    std::random_device _rd;
    cl_context context;
    CloudCLManager(){};
    ~CloudCLManager();
    void initialize(Texture3D* textureids);
    void densityUpdate();
    void lightMapUpdate(const glm::vec3& lightDir);

    cl_mem debug1, debug2, debug3, debug4;
    float* A;
    float* B;
    float* C;
    float* D;
    const int count = 128 * 128*128;
};
