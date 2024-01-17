#include "Texture3D.h"

Texture3D::Texture3D(){}
Texture3D::~Texture3D(){}
void Texture3D::initialize3D(GLenum internalFormat, uint64 w, uint64 h, uint64 d)
{
  _width = w;
  _height = h;
  _depth = d;

  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_3D, _id);

  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, _width, _height, _depth, 0, GL_RGBA, GL_FLOAT, NULL);

  glBindTexture(GL_TEXTURE_3D, 0);
}