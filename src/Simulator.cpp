#include "Simulator.h"
#include "Camera.h"
#include "ParticleScene.h"

Simulator::~Simulator(){}

void Simulator::initialize(uint32 particlecount)
{
  this->_particleScene = std::make_unique<ParticleScene>(particlecount);
  this->_particleScene->initialize();
}

void Simulator::draw(void)
{
  _particleScene->draw();
}

void Simulator::update(float delta)
{
  _particleScene->update(delta);
}