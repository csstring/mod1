#include "StableFluidsCLManager.h"
#include <fstream>
#include <sstream>
#include <random>
#include "EnumHeader.h"
#include "Texture2D.h"

void StableFluidsCLManager::initialize(Texture2D* textureids)
{
  ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
  if (ret != CL_SUCCESS) {
      std::cout << "Error getting platform ID: " << ret << std::endl;
      ft_assert("clGetPlatformIDs");
  }

  ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
  if (ret != CL_SUCCESS || ret_num_devices == 0) {
      std::cout << "Error getting device IDs or no devices found: " << ret << std::endl;
      ft_assert("clGetDeviceIDs");
  }

  CGLContextObj glContext = CGLGetCurrentContext();
  if (glContext == NULL) {
      ft_assert("CGLGetCurrentContext returned NULL");
  }
  CGLShareGroupObj shareGroup = CGLGetShareGroup(glContext);
  if (shareGroup == NULL) {
      ft_assert("CGLGetShareGroup returned NULL");
  }
  cl_context_properties properties[] = {
    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)shareGroup,0
  };

  context = clCreateContext(properties, 1, &device_id, nullptr, nullptr, &ret);
  if (ret != CL_SUCCESS) {
      std::cout << "Error creating context: " << ret << std::endl;
      ft_assert("clCreateContext");
  }

  command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
  if (ret != 0){
    std::cout << ret << std::endl;
    ft_assert("clCreateCommandQueue");
  }
  
  CLTextureID[TEXTUREID::VELOCITY] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::VELOCITY].getID(), &ret);
  CLTextureID[TEXTUREID::VELOCITYTMP] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::VELOCITYTMP].getID(), &ret);
  CLTextureID[TEXTUREID::VORTICITY] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::VORTICITY].getID(), &ret);
  CLTextureID[TEXTUREID::PRESSURE] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::PRESSURE].getID(), &ret);
  CLTextureID[TEXTUREID::PRESSURETMP] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::PRESSURETMP].getID(), &ret);
  CLTextureID[TEXTUREID::DIVERGENCE] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::DIVERGENCE].getID(), &ret);
  CLTextureID[TEXTUREID::DENSITY] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::DENSITY].getID(), &ret);
  CLTextureID[TEXTUREID::DENSITYTMP] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::DENSITYTMP].getID(), &ret);
  samplerLinearWrap = clCreateSampler(context, CL_FALSE, CL_ADDRESS_REPEAT, CL_FILTER_LINEAR, NULL);
  if (ret != 0){
    std::cout << ret << std::endl;
    ft_assert("clCreateFromGLBuffer");
  }

  _programs.resize(2);
  _programs[SFKernelFunc::SOURCING].initialize(context, device_id, "./stableFluidsScene/kernelSource/sourcing.cl", "sourcing");
  // _programs[SFKernelFunc::ADVECT].initialize(context, device_id, "./stableFluidsScene/kernelSource/advect.cl", "advect");
}

void StableFluidsCLManager::sourcing(const glm::vec4& cursor)
{
  cl_kernel kernel = _programs[SFKernelFunc::SOURCING]._kernel;
  static int color = 0;
  glm::vec4 vel(0.0f);
  static const std::vector<glm::vec4> rainbow = {
                {1.0f, 0.0f, 0.0f, 1.0f},  // Red
                {1.0f, 0.65f, 0.0f, 1.0f}, // Orange
                {1.0f, 1.0f, 0.0f, 1.0f},  // Yellow
                {0.0f, 1.0f, 0.0f, 1.0f},  // Green
                {0.0f, 0.0f, 1.0f, 1.0f},  // Blue
                {0.3f, 0.0f, 0.5f, 1.0f},  // Indigo
                {0.5f, 0.0f, 1.0f, 1.0f}   // Violet/Purple
            };
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITY]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::DENSITY]);
  clSetKernelArg(kernel, 2, sizeof(cl_sampler), &samplerLinearWrap);
  clSetKernelArg(kernel, 3, 4 * sizeof(float), &cursor);
  clSetKernelArg(kernel, 4, 4 * sizeof(float), &vel);
  clSetKernelArg(kernel, 5, 4 * sizeof(float), &rainbow[(color++) % 7]);
  size_t global_work_size[2] = {static_cast<size_t>(WINDOW_WITH), static_cast<size_t>(WINDOW_HEIGHT)}; // Replace 1024 with your actual dimensions
  ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    std::cout << ret << std::endl;
    ft_assert("Error enqueuing kernel");
  }
  clFinish(command_queue);
}



StableFluidsCLManager::~StableFluidsCLManager()
{
  ret = clFlush(command_queue);
  ret = clFinish(command_queue);
  this->_programs.clear();
  for (int i =0; i <8; ++i){
    clReleaseMemObject(this->CLTextureID[i]);
  }
  ret = clReleaseCommandQueue(command_queue);
  ret = clReleaseContext(context);
}