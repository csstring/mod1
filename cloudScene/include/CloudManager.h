#pragma once
#include "Common.h"
#include "CloudModel.h"

//thread
class Camera;

class CloudManager : Noncopyable
{
  private:
    void initTexture();

  public:
    CloudModel _cloud;
    
  public:
    CloudManager(){};
    ~CloudManager();

    void initialize();
    void draw();
};