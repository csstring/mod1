#include "CLManager.h"
#include <fstream>
#include <sstream>
#include <OpenGL/OpenGL.h>
#include <random>

void CLManager::initialize(uint32 VBO, uint64 count)
{
  ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
  if (ret != CL_SUCCESS) {
      std::cout << "Error getting platform ID: " << ret << std::endl;
      ft_assert("clGetPlatformIDs");
  }

  // Get device IDs
  ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
  if (ret != CL_SUCCESS || ret_num_devices == 0) {
      std::cout << "Error getting device IDs or no devices found: " << ret << std::endl;
      ft_assert("clGetDeviceIDs");
  }

  // Verify GL context and share group
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
  clVBO = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, VBO, &ret);
  if (ret != 0){
    std::cout << ret << std::endl;
    ft_assert("clCreateFromGLBuffer");
  }

  _programs.resize(4);
  _programs[MAIN_ROOP].initialize(context, device_id, "./kernelSource/particle_sys.cl", "particle_sys");
  _programs[INIT_CIRCLE].initialize(context, device_id, "./kernelSource/init_circle.cl", "init_circle");
  _programs[INIT_PLANE].initialize(context, device_id, "./kernelSource/init_plane.cl", "init_plane");
  _programs[GENERATOR].initialize(context, device_id, "./kernelSource/particle_generator.cl", "particle_generator");

  global_item_size = count;
}

void CLManager::initCircle()
{
  cl_kernel kernel = _programs[INIT_CIRCLE]._kernel;
  uint32 seed = _rd();

  clSetKernelArg(kernel, 0, sizeof(cl_mem), &clVBO);
  clSetKernelArg(kernel, 1, sizeof(uint32), &seed);
  clEnqueueNDRangeKernel(command_queue, kernel, 1, nullptr, &global_item_size, nullptr, 0, nullptr, nullptr);
  clFinish(command_queue);
}

void CLManager::initPlane()
{
  cl_kernel kernel = _programs[INIT_PLANE]._kernel;
  uint32 seed = _rd();

  clSetKernelArg(kernel, 0, sizeof(cl_mem), &clVBO);
  clSetKernelArg(kernel, 1, sizeof(uint32), &seed);
  clEnqueueNDRangeKernel(command_queue, kernel, 1, nullptr, &global_item_size, nullptr, 0, nullptr, nullptr);
  clFinish(command_queue);
}

void CLManager::particleGenerate(float dt, const glm::vec4& gravity)
{
  dt *= 0.4;
  const size_t geCount = 5000;
  cl_kernel generator = _programs[GENERATOR]._kernel;
  uint32 seed = _rd();
  clSetKernelArg(generator, 0, sizeof(cl_mem), &clVBO);
  clSetKernelArg(generator, 1, sizeof(uint32), &seed);
  clSetKernelArg(generator, 2, 4 * sizeof(float), &gravity);
  clSetKernelArg(generator, 3, sizeof(float), &dt);
  clEnqueueNDRangeKernel(command_queue, generator, 1, nullptr, &geCount, nullptr, 0, nullptr, nullptr);
  clFinish(command_queue);
}

void CLManager::update(float dt, const glm::vec4& gravity, int32 drawCount)
{
    const size_t totalParticles = drawCount;  // Total number of particles
    dt *= 0.4;
    cl_kernel kernel = _programs[MAIN_ROOP]._kernel;

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &clVBO);
    clSetKernelArg(kernel, 1, sizeof(float), &dt);
    clSetKernelArg(kernel, 2, 4 * sizeof(float), &gravity);

    clEnqueueNDRangeKernel(command_queue, kernel, 1, nullptr, &totalParticles, nullptr, 0, nullptr, nullptr);

    clFinish(command_queue);
}

CLManager::~CLManager()
{
  ret = clFlush(command_queue);
  ret = clFinish(command_queue);
  this->_programs.clear();
  ret = clReleaseMemObject(clVBO);
  ret = clReleaseCommandQueue(command_queue);
  ret = clReleaseContext(context);
}