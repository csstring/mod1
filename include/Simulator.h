#pragma once
#include "Common.h"
#include "EnumHeader.h"
#include "Scene.h"
#include <memory>

class Simulator : Noncopyable
{
    public:
        std::unique_ptr<Scene> _particleScene;
        int32 _speed;

    public:
        Simulator() : _speed(0){};
        ~Simulator();
        
        void initialize(uint32 particlecount);
        void update(float delta);
        void draw(void);
};