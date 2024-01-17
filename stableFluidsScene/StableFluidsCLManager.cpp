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
  
  CLTextureID[TEXTUREID::VELOCITYID] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::VELOCITYID].getID(), &ret);
  CLTextureID[TEXTUREID::VELOCITYTMPID] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::VELOCITYTMPID].getID(), &ret);
  CLTextureID[TEXTUREID::VORTICITYID] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::VORTICITYID].getID(), &ret);
  CLTextureID[TEXTUREID::PRESSUREID] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::PRESSUREID].getID(), &ret);
  CLTextureID[TEXTUREID::PRESSURETMPID] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::PRESSURETMPID].getID(), &ret);
  CLTextureID[TEXTUREID::DIVERGENCEID] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::DIVERGENCEID].getID(), &ret);
  CLTextureID[TEXTUREID::DIVERGENCETMPID] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::DIVERGENCETMPID].getID(), &ret);
  CLTextureID[TEXTUREID::DENSITYID] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::DENSITYID].getID(), &ret);
  CLTextureID[TEXTUREID::DENSITYTMPID] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureids[TEXTUREID::DENSITYTMPID].getID(), &ret);
  samplerLinearWrap = clCreateSampler(context, CL_FALSE, CL_ADDRESS_REPEAT, CL_FILTER_LINEAR, NULL);
  samplerPointWrap = clCreateSampler(context, CL_FALSE, CL_ADDRESS_REPEAT, CL_FILTER_NEAREST, NULL);
  if (ret != 0){
    std::cout << ret << std::endl;
    ft_assert("clCreateFromGLBuffer");
  }

  _programs.resize(10);
  _programs[SFKernelFunc::SOURCING].initialize(context, device_id, "./stableFluidsScene/kernelSource/sourcing.cl", "sourcing");
  _programs[SFKernelFunc::ADVECT].initialize(context, device_id, "./stableFluidsScene/kernelSource/advect.cl", "advect");
  _programs[SFKernelFunc::COPY].initialize(context, device_id, "./stableFluidsScene/kernelSource/copyTexture.cl", "copyTexture");
  _programs[SFKernelFunc::PRINT].initialize(context, device_id, "./stableFluidsScene/kernelSource/printTexture.cl", "printTexture");
  _programs[SFKernelFunc::DIVERGENCE].initialize(context, device_id, "./stableFluidsScene/kernelSource/divergence.cl", "divergence");
  _programs[SFKernelFunc::JACOBI].initialize(context, device_id, "./stableFluidsScene/kernelSource/jacobi.cl", "jacobi");
  _programs[SFKernelFunc::APPLYPRESSURE].initialize(context, device_id, "./stableFluidsScene/kernelSource/applyPressure.cl", "applyPressure");
  _programs[SFKernelFunc::DIFFUSE].initialize(context, device_id, "./stableFluidsScene/kernelSource/diffuse.cl", "diffuse");
  _programs[SFKernelFunc::COMPUTEVORTICITY].initialize(context, device_id, "./stableFluidsScene/kernelSource/computeVorticity.cl", "computeVorticity");
  _programs[SFKernelFunc::CONFINEVORTICITY].initialize(context, device_id, "./stableFluidsScene/kernelSource/confineVorticity.cl", "confineVorticity");

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
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[src]);
  clSetKernelArg(kernel, 1, sizeof(int32), &global_work_size[0]);
  clSetKernelArg(kernel, 2, sizeof(int32), &global_work_size[1]);
  clSetKernelArg(kernel, 3, sizeof(cl_sampler), &samplerPointWrap);
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
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[src]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[dst]);
  ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    std::cout << ret << std::endl;
    ft_assert("Error enqueuing kernel");
  }
  clFinish(command_queue);
}

void StableFluidsCLManager::advection(float dt)
{
  cl_kernel kernel = _programs[SFKernelFunc::ADVECT]._kernel;
  copyImage(TEXTUREID::VELOCITYID, TEXTUREID::VELOCITYTMPID);
  copyImage(TEXTUREID::DENSITYID, TEXTUREID::DENSITYTMPID);
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYTMPID]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::DENSITYTMPID]);
  clSetKernelArg(kernel, 2, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYID]);
  clSetKernelArg(kernel, 3, sizeof(cl_mem), &CLTextureID[TEXTUREID::DENSITYID]);
  clSetKernelArg(kernel, 4, sizeof(cl_sampler), &samplerPointWrap);
  clSetKernelArg(kernel, 5, sizeof(cl_sampler), &samplerLinearWrap);
  clSetKernelArg(kernel, 6, sizeof(float), &dt);
  {
    clSetKernelArg(kernel, 7, sizeof(cl_mem), &debug1);
    clSetKernelArg(kernel, 8, sizeof(cl_mem), &debug2);
    clSetKernelArg(kernel, 9, sizeof(cl_mem), &debug3);
    clSetKernelArg(kernel, 10, sizeof(cl_mem), &debug4);
  }
  ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    std::cout << ret << std::endl;
    ft_assert("Error enqueuing kernel");
  }
  clFinish(command_queue);
  // clEnqueueReadBuffer(command_queue, debug1, CL_TRUE, 0, 
  //           count * sizeof(float), A, 0, NULL, NULL);
  // clEnqueueReadBuffer(command_queue, debug2, CL_TRUE, 0, 
  //           count * sizeof(float), B, 0, NULL, NULL);
  // clEnqueueReadBuffer(command_queue, debug3, CL_TRUE, 0, 
  //           count * sizeof(float), C, 0, NULL, NULL);
  // clEnqueueReadBuffer(command_queue, debug4, CL_TRUE, 0, 
  //           count * sizeof(float), D, 0, NULL, NULL);
  // for (int i =1920; i < 1920 * 2; ++i){
  //   std::cout << "pos.x : "<<A[i] << "  pos.y : " << B[i] << " dx.x : " << C[i] << "  dx.y : " << D[i] << std::endl;
  // }
}

void StableFluidsCLManager::sourcing(const glm::vec4& cursor)
{
  cl_kernel kernel = _programs[SFKernelFunc::SOURCING]._kernel;
  static int color = 0;
  Camera cam = Camera::getInstance();
  static int32 x = cam._lastX / 4;
  static int32 y = cam._lastY/4;

  glm::vec4 vel(cam._lastX / 4 - x, cam._lastY/4 - y, 0.0f, 0.0f);
  x = cam._lastX / 4;
  y = cam._lastY/4;
  if (cam._clickOn == true){
    vel.w = 1;
  }
  copyImage(TEXTUREID::VELOCITYID, TEXTUREID::VELOCITYTMPID);
  copyImage(TEXTUREID::DENSITYID, TEXTUREID::DENSITYTMPID);

  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYTMPID]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::DENSITYTMPID]);
  clSetKernelArg(kernel, 2, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYID]);
  clSetKernelArg(kernel, 3, sizeof(cl_mem), &CLTextureID[TEXTUREID::DENSITYID]);
  clSetKernelArg(kernel, 4, 4 * sizeof(float), &vel);
  clSetKernelArg(kernel, 5, 4 * sizeof(float), &rainbow[(color++) % 7]);
  clSetKernelArg(kernel, 6, sizeof(int32), &x);
  clSetKernelArg(kernel, 7, sizeof(int32), &y);
// {

//   clSetKernelArg(kernel, 9, sizeof(cl_mem), &debug1);
//   clSetKernelArg(kernel, 10, sizeof(cl_mem), &debug2);
//   clSetKernelArg(kernel, 11, sizeof(cl_mem), &debug3);
//   clSetKernelArg(kernel, 12, sizeof(cl_mem), &debug4);
// }

  ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    std::cout << ret << std::endl;
    ft_assert("Error enqueuing kernel");
  }
  clFinish(command_queue);

  // if (color == 1){

  // clEnqueueReadBuffer(command_queue, debug1, CL_TRUE, 0, 
  //           count * sizeof(float), A, 0, NULL, NULL);
  // clEnqueueReadBuffer(command_queue, debug2, CL_TRUE, 0, 
  //           count * sizeof(float), B, 0, NULL, NULL);
  // clEnqueueReadBuffer(command_queue, debug3, CL_TRUE, 0, 
  //           count * sizeof(float), C, 0, NULL, NULL);
  // clEnqueueReadBuffer(command_queue, debug4, CL_TRUE, 0, 
  //           count * sizeof(float), D, 0, NULL, NULL);
  // for (int i =1920; i < 1920 * 2; ++i){
  //   std::cout << "width : "<<A[i] << "  x : " << B[i] << " y : " << C[i] << "  a : " << D[i] << std::endl;
  // }
  // }
}

void StableFluidsCLManager::vorticity(float dt)
{
  cl_kernel kernel = _programs[SFKernelFunc::COMPUTEVORTICITY]._kernel;
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::VORTICITYID]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYID]);
  ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    std::cout << ret << std::endl;
    ft_assert("Error enqueuing kernel");
  }
  clFinish(command_queue);

  copyImage(TEXTUREID::VELOCITYID, TEXTUREID::VELOCITYTMPID);
  kernel = _programs[SFKernelFunc::CONFINEVORTICITY]._kernel;
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::VORTICITYID]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYTMPID]);
  clSetKernelArg(kernel, 2, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYID]);
  clSetKernelArg(kernel, 3, sizeof(float), &dt);
  ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    std::cout << ret << std::endl;
    ft_assert("Error enqueuing kernel");
  }
  clFinish(command_queue);
}

void StableFluidsCLManager::diffuse(float dt,float viscosity)
{
  cl_kernel kernel = _programs[SFKernelFunc::DIFFUSE]._kernel;
  copyImage(TEXTUREID::VELOCITYID, TEXTUREID::VELOCITYTMPID);
  copyImage(TEXTUREID::DENSITYID, TEXTUREID::DENSITYTMPID);

  for (int i =0; i < 10; ++i)
  {
    if (i % 2 == 0)
    {
      clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYID]);
      clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::DENSITYID]);
      clSetKernelArg(kernel, 2, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYTMPID]);
      clSetKernelArg(kernel, 3, sizeof(cl_mem), &CLTextureID[TEXTUREID::DENSITYTMPID]);
    }
    else 
    {
      clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYTMPID]);
      clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::DENSITYTMPID]);
      clSetKernelArg(kernel, 2, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYID]);
      clSetKernelArg(kernel, 3, sizeof(cl_mem), &CLTextureID[TEXTUREID::DENSITYID]);
    }
    clSetKernelArg(kernel, 4, sizeof(float), &viscosity);
    clSetKernelArg(kernel, 5, sizeof(float), &dt);
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
      std::cout << ret << std::endl;
      ft_assert("Error enqueuing kernel");
    }
    clFinish(command_queue);
  }
}

void StableFluidsCLManager::projection()
{
  cl_kernel kernel = _programs[SFKernelFunc::DIVERGENCE]._kernel;

  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::DIVERGENCEID]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYID]);
  clSetKernelArg(kernel, 2, sizeof(cl_mem), &CLTextureID[TEXTUREID::PRESSUREID]);
  clSetKernelArg(kernel, 3, sizeof(cl_mem), &CLTextureID[TEXTUREID::PRESSURETMPID]);

  ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    std::cout << ret << std::endl;
    ft_assert("Error enqueuing kernel");
  }
  clFinish(command_queue);

  kernel = _programs[SFKernelFunc::JACOBI]._kernel;
  for (int i =0; i < 100; ++i)
  {
    if (i % 2 == 0)
    {
      clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::DIVERGENCEID]);
      clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::PRESSUREID]);
      clSetKernelArg(kernel, 2, sizeof(cl_mem), &CLTextureID[TEXTUREID::PRESSURETMPID]);
    }
    else 
    {
      clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::DIVERGENCEID]);
      clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::PRESSURETMPID]);
      clSetKernelArg(kernel, 2, sizeof(cl_mem), &CLTextureID[TEXTUREID::PRESSUREID]);
    }
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
      std::cout << ret << std::endl;
      ft_assert("Error enqueuing kernel");
    }
    clFinish(command_queue);
  }

  kernel = _programs[SFKernelFunc::APPLYPRESSURE]._kernel;
  copyImage(TEXTUREID::VELOCITYID, TEXTUREID::VELOCITYTMPID);
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYID]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[TEXTUREID::VELOCITYTMPID]);
  clSetKernelArg(kernel, 2, sizeof(cl_mem), &CLTextureID[TEXTUREID::PRESSUREID]);
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
  for (int i =0; i <10; ++i){
    clReleaseMemObject(this->CLTextureID[i]);
  }
  ret = clReleaseCommandQueue(command_queue);
  ret = clReleaseContext(context);
}