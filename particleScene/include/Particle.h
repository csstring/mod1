#pragma once
#include "Common.h"
struct alignas(16) Particle
{
  glm::vec4 _position;
  glm::vec4 _velocity;
  float _life;
};
