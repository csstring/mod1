#include "Simulator.h"
#include "Camera.h"
#include "ParticleScene.h"
#include "StableFluidsScene.h"

Simulator::~Simulator(){}

void Simulator::initialize(uint32 particlecount)
{
  // this->_particleScene = std::make_unique<ParticleScene>(particlecount);
  // this->_particleScene->initialize();
  _stableFluidsScene = std::make_unique<StableFluidsScene>();
  _stableFluidsScene->initialize();
}

void Simulator::draw(void)
{
  _stableFluidsScene->draw();
  // _particleScene->draw();
}

void Simulator::update(float delta)
{
  _stableFluidsScene->update(delta);
  // _particleScene->update(delta);
}