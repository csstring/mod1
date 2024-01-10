#include "ParticleManager.h"
#include "Camera.h"
#include "Particle.h"

ParticleManager::~ParticleManager()
{
  glDeleteVertexArrays(1, &_VAO);
  glDeleteBuffers(1, &_VBO);
}

void ParticleManager::initialize()
{
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);
  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, _particleCount * sizeof(Particle), nullptr, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, _position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, _velocity));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, _life));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void ParticleManager::draw(uint32 drawCount)
{
  glBindVertexArray(_VAO);
  glDrawArrays(GL_POINTS, 0, drawCount);
  glBindVertexArray(0);
}
