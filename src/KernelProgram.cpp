#include "KernelProgram.h"
#include <fstream>
#include <sstream>

void KernelProgram::initialize(cl_context context, cl_device_id deviceID, std::string codePath, std::string name)
{
  std::ifstream sourceCodeStream(codePath, std::ios::in);
  std::string kernerCode;
  int32 ret;

  if (sourceCodeStream.is_open())
  {
    std::stringstream ss;
    ss << sourceCodeStream.rdbuf();
    kernerCode = ss.str();
    sourceCodeStream.close();
  }
  else
    ft_assert("kernel code open fail");

  const char* code = kernerCode.c_str();
  size_t tmplength = kernerCode.size();
  if (context == nullptr)
    ft_assert("context null");
  _program = clCreateProgramWithSource(context, 1, &code, &tmplength, &ret);
  if (ret != 0){
    std::cout << ret << std::endl;
    ft_assert("clCreateProgramWithSource");
  }
  ret = clBuildProgram(_program, 1, &deviceID, nullptr, nullptr, nullptr);
  if (ret != 0){
    std::cout << ret << std::endl;
    ft_assert("clBuildProgram");
  }
  _kernel = clCreateKernel(_program, name.c_str(), &ret);
  if (ret != 0){
    std::cout << ret << std::endl;
    ft_assert("clCreateKernel");
  } 
}

KernelProgram::~KernelProgram()
{
  clReleaseKernel(_kernel);
  clReleaseProgram(_program);
}