#pragma once
#include <OpenCL/opencl.h>
#include "Common.h"
enum KernelFunc {
  MAIN_ROOP,
  INIT_CIRCLE,
  INIT_PLANE,
  GENERATOR
};

class KernelProgram
{
  private:
    cl_program _program;
    
  public:
    cl_kernel _kernel;

    KernelProgram(){};
    ~KernelProgram();
    void initialize(cl_context context, cl_device_id deviceID, std::string codePath, std::string name);
};