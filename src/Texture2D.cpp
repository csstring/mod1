#include "Texture2D.h"

Texture2D::Texture2D(){}
Texture2D::~Texture2D(){}
void Texture2D::initialize(GLenum internalFormat, uint64 w, uint64 h)
{
  _width = w;
  _height = h;
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);

  glBindTexture(GL_TEXTURE_2D, 0);
}