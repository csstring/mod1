#include "CloudCLManager.h"
#include <fstream>
#include <sstream>
#include <random>
#include "Texture3D.h"
#include "Camera.h"

void CloudCLManager::initialize(Texture3D* textureids)
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
  CLTextureID[ClOUDTEXTUREID::LIGHTMAPID] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_3D, 0, textureids[ClOUDTEXTUREID::LIGHTMAPID].getID(), &ret);
  CLTextureID[ClOUDTEXTUREID::CLOUDDENSITYID] = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_3D, 0, textureids[ClOUDTEXTUREID::CLOUDDENSITYID].getID(), &ret);

  samplerLinearWrap = clCreateSampler(context, CL_FALSE, CL_ADDRESS_REPEAT, CL_FILTER_LINEAR, NULL);
  samplerPointWrap = clCreateSampler(context, CL_FALSE, CL_ADDRESS_REPEAT, CL_FILTER_NEAREST, NULL);

  _programs.resize(2);
  _programs[CLOUDKernelFunc::CLOUDLIGHTING].initialize(context, device_id, "./cloudScene/kernelSource/cloudLighting.cl", "cloudLighting");
  _programs[CLOUDKernelFunc::CLOUDDENSITY].initialize(context, device_id, "./cloudScene/kernelSource/cloudDensity.cl", "cloudDensity");

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

void CloudCLManager::densityUpdate()
{
  cl_kernel kernel = _programs[CLOUDKernelFunc::CLOUDDENSITY]._kernel;
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[ClOUDTEXTUREID::CLOUDDENSITYID]);
  ret = clEnqueueNDRangeKernel(command_queue, kernel, 3, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    std::cout << ret << std::endl;
    ft_assert("Error enqueuing kernel");
  }
  clFinish(command_queue);
}

void CloudCLManager::lightMapUpdate(const glm::vec3& lightDir)
{
  cl_kernel kernel = _programs[CLOUDKernelFunc::CLOUDLIGHTING]._kernel;
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLTextureID[ClOUDTEXTUREID::CLOUDDENSITYID]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &CLTextureID[ClOUDTEXTUREID::LIGHTMAPID]);
  clSetKernelArg(kernel, 2, 3*sizeof(float), &lightDir);
  clSetKernelArg(kernel, 3, sizeof(cl_sampler), &samplerLinearWrap);
  ret = clEnqueueNDRangeKernel(command_queue, kernel, 3, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    std::cout << ret << std::endl;
    ft_assert("Error enqueuing kernel");
  }
  clFinish(command_queue);
}

CloudCLManager::~CloudCLManager()
{
  ret = clFlush(command_queue);
  ret = clFinish(command_queue);
  this->_programs.clear();
  for (int i =0; i <2; ++i){
    clReleaseMemObject(this->CLTextureID[i]);
  }
  ret = clReleaseCommandQueue(command_queue);
  ret = clReleaseContext(context);
}