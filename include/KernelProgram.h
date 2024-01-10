#pragma once
#include "Common.h"

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