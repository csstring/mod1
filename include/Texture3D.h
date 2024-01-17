#pragma once
#include "Common.h"

class Texture3D
{
  private:
    uint32 _id;
    uint64 _width;
    uint64 _height;
    uint64 _depth;

  public:
    Texture3D();
    ~Texture3D();
    void initialize3D(GLenum internalFormat, uint64 w, uint64 h, uint64 d);
    uint32 getID(){return _id;};
    uint32* getAddress(){return &_id;};
    uint64 getWidth(){return _width;};
    uint64 getHeight(){return _height;};
};
