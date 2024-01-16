#include "Simulator.h"
#include "Camera.h"
#include "ParticleScene.h"
#include "StableFluidsScene.h"

Simulator::~Simulator(){}

void Simulator::initialize(uint32 particlecount)
{
  _particleScene = std::make_unique<ParticleScene>(particlecount);
  _particleScene->initialize();
  _stableFluidsScene = std::make_unique<StableFluidsScene>();
  _stableFluidsScene->initialize();
  _currentScene = _particleScene.get();
}

void Simulator::draw(void)
{
  _currentScene->draw();
}

void Simulator::update(float delta)
{
  _currentScene->update(delta);
}