#pragma once
class Scene
{
  public:
    virtual void initialize() = 0;
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual ~Scene(){};
};
