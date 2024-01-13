#include "StableFluidsCLManager.h"
#include <fstream>
#include <sstream>
#include <random>
#include "Texture2D.h"
#include "Camera.h"

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
  samplerLinearWrap = clCreateSampler(context, CL_FALSE, CL_ADDRESS_CLAMP, CL_FILTER_NEAREST, NULL);
  if (ret != 0){
    std::cout << ret << std::endl;
    ft_assert("clCreateFromGLBuffer");
  }

  _programs.resize(4);
  _programs[SFKernelFunc::SOURCING].initialize(context, device_id, "./stableFluidsScene/kernelSource/sourcing.cl", "sourcing");
  _programs[SFKernelFunc::ADVECT].initialize(context, device_id, "./stableFluidsScene/kernelSource/advect.cl", "advect");
  _programs[SFKernelFunc::COPY].initialize(context, device_id, "./stableFluidsScene/kernelSource/copyTexture.cl", "copyTexture");
  _programs[SFKernelFunc::PRINT].initialize(context, device_id, "./stableFluidsScene/kernelSource/printTexture.cl", "printTexture");

  debug1 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            count * sizeof(float), NULL, &ret);
  debug2 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            count * sizeof(float), NULL, &ret);
  debug3 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            count * sizeof(float), NULL, &ret);
  debug4 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            count * sizeof(float), NULL, &ret);
  A = (float*)malloc(sizeof(float)*count);
  B = (float*)malloc(sizeof(float)*count);
  C = (float*)malloc(sizeof(float)*count);
  D = (float*)malloc(sizeof(float)*count);

}

void StableFluidsCLManager::printTextureColor(TEXTUREID src)
{
  cl_kernel kernel = _programs[SFKernelFunc::PRINT]._kernel;
  size_t global_work_size[2] = {static_cast<size_t>(WINDOW_WITH), static_cast<size_t>(WINDOW_HEIGHT)};
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[src]);
  clSetKernelArg(kernel, 1, sizeof(int32), &global_work_size[0]);
  clSetKernelArg(kernel, 2, sizeof(int32), &global_work_size[1]);
  clSetKernelArg(kernel, 3, sizeof(cl_sampler), &samplerLinearWrap);
  clSetKernelArg(kernel, 4, sizeof(cl_mem), &debug1);
  clSetKernelArg(kernel, 5, sizeof(cl_mem), &debug2);
  clSetKernelArg(kernel, 6, sizeof(cl_mem), &debug3);
  clSetKernelArg(kernel, 7, sizeof(cl_mem), &debug4);
  clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
  clFinish(command_queue);

  clEnqueueReadBuffer(command_queue, debug1, CL_TRUE, 0, 
            count * sizeof(float), A, 0, NULL, NULL);
  clEnqueueReadBuffer(command_queue, debug2, CL_TRUE, 0, 
            count * sizeof(float), B, 0, NULL, NULL);
  clEnqueueReadBuffer(command_queue, debug3, CL_TRUE, 0, 
            count * sizeof(float), C, 0, NULL, NULL);
  clEnqueueReadBuffer(command_queue, debug4, CL_TRUE, 0, 
            count * sizeof(float), D, 0, NULL, NULL);
  for (int i =0; i < 100; ++i){
    std::cout << "r : "<<A[i] << "  g : " << B[i] << " b : " << C[i] << "  a : " << D[i] << std::endl;
  }
}

void StableFluidsCLManager::copyImage(TEXTUREID src, TEXTUREID dst)
{
  cl_kernel kernel = _programs[SFKernelFunc::COPY]._kernel;
  size_t global_work_size[2] = {static_cast<size_t>(WINDOW_WITH), static_cast<size_t>(WINDOW_HEIGHT)};
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[src]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[dst]);
  clSetKernelArg(kernel, 2, sizeof(cl_sampler), &samplerLinearWrap);
  ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    std::cout << ret << std::endl;
    ft_assert("Error enqueuing kernel");
  }
  clFinish(command_queue);
}

void StableFluidsCLManager::sourcing(const glm::vec4& cursor)
{
  cl_kernel kernel = _programs[SFKernelFunc::SOURCING]._kernel;
  static int color = 0;
  glm::vec4 vel(0.0f);
  Camera cam = Camera::getInstance();
  size_t global_work_size[2] = {static_cast<size_t>(WINDOW_WITH), static_cast<size_t>(WINDOW_HEIGHT)}; // Replace 1024 with your actual dimensions
  // std::cout << "x : "<<cam._lastX << " y : " << cam._lastY << std::endl;
  int32 x = cam._lastX;
  int32 y = WINDOW_HEIGHT - cam._lastY;
  copyImage(TEXTUREID::VELOCITY, TEXTUREID::VELOCITYTMP);
  copyImage(TEXTUREID::DENSITY, TEXTUREID::DENSITYTMP);

  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYTMP]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::DENSITYTMP]);
  clSetKernelArg(kernel, 2, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITY]);
  clSetKernelArg(kernel, 3, sizeof(cl_mem), &CLTextureID[TEXTUREID::DENSITY]);
  clSetKernelArg(kernel, 4, 4 * sizeof(float), &vel);
  clSetKernelArg(kernel, 5, 4 * sizeof(float), &rainbow[(color++) % 7]);
  clSetKernelArg(kernel, 6, sizeof(cl_sampler), &samplerLinearWrap);
  clSetKernelArg(kernel, 7, sizeof(int32), &x);
  clSetKernelArg(kernel, 8, sizeof(int32), &y);
{

  clSetKernelArg(kernel, 9, sizeof(cl_mem), &debug1);
  clSetKernelArg(kernel, 10, sizeof(cl_mem), &debug2);
  clSetKernelArg(kernel, 11, sizeof(cl_mem), &debug3);
  clSetKernelArg(kernel, 12, sizeof(cl_mem), &debug4);
}

  ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    std::cout << ret << std::endl;
    ft_assert("Error enqueuing kernel");
  }
  clFinish(command_queue);
  // std::cout << count << std::endl;
  if (color == 1){

  clEnqueueReadBuffer(command_queue, debug1, CL_TRUE, 0, 
            count * sizeof(float), A, 0, NULL, NULL);
  clEnqueueReadBuffer(command_queue, debug2, CL_TRUE, 0, 
            count * sizeof(float), B, 0, NULL, NULL);
  clEnqueueReadBuffer(command_queue, debug3, CL_TRUE, 0, 
            count * sizeof(float), C, 0, NULL, NULL);
  clEnqueueReadBuffer(command_queue, debug4, CL_TRUE, 0, 
            count * sizeof(float), D, 0, NULL, NULL);
  for (int i =1920; i < 1920 * 2; ++i){
    std::cout << "width : "<<A[i] << "  x : " << B[i] << " y : " << C[i] << "  a : " << D[i] << std::endl;
  }
  }
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