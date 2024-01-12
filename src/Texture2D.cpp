#include "Texture2D.h"

Texture2D::Texture2D(){}
Texture2D::~Texture2D(){}
void Texture2D::initialize(GLenum internalFormat)
{
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, WINDOW_WITH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

  glBindTexture(GL_TEXTURE_2D, 0);
  int a = 0;
  const int tmpa = a;
  a = tmpa + 1;
  
}