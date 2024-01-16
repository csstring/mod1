#pragma once
#include "Common.h"
#include "EnumHeader.h"
#include "Scene.h"
#include <memory>

class Simulator : Noncopyable
{
    public:
        std::unique_ptr<Scene> _particleScene;
        std::unique_ptr<Scene> _stableFluidsScene;
        Scene* _currentScene;
        int32 _speed;

    public:
        Simulator() : _speed(1){};
        ~Simulator();
        
        void initialize(uint32 particlecount);
        void update(float delta);
        void draw(void);
};