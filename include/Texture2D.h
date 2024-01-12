#pragma once
#include "Common.h"

class Texture2D
{
  private:
    uint32 _id;

  public:
    Texture2D();
    ~Texture2D();
    void initialize(GLenum internalFormat);
    uint32 getID(){return _id;};
    uint32* getAddress(){return &_id;};
};
