#pragma once
#include "Common.h"

class Texture2D
{
  private:
    uint32 _id;
    uint64 _width;
    uint64 _height;

  public:
    Texture2D();
    ~Texture2D();
    void initialize(GLenum internalFormat, uint64 w, uint64 h);
    uint32 getID(){return _id;};
    uint32* getAddress(){return &_id;};
    uint64 getWidth(){return _width;};
    uint64 getHeight(){return _height;};
};
