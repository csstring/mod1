#pragma once
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ostream>
#include <cmath>
#include <filesystem>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <OpenCL/opencl.h>
#include <OpenGL/OpenGL.h>
#include "GLM/glm.hpp"
#include "GLM/gtx/string_cast.hpp"

#define GL_SILENCE_DEPRECATION
typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef signed long         int64;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long       uint64;

constexpr float WINDOW_WITH = 1920;
constexpr float WINDOW_HEIGHT = 1080;
constexpr float ASPECT_RATIO = WINDOW_WITH / WINDOW_HEIGHT;

class Noncopyable
{
    public:
        inline ~Noncopyable(void){}
    protected:
        inline Noncopyable(void){}
    private:
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable(Noncopyable&) = delete;
        Noncopyable& operator=(const Noncopyable&) = delete;
        Noncopyable& operator=(Noncopyable&) = delete;
};

//debug
void ft_assert(const std::string& exec);

//utility
inline std::chrono::steady_clock::time_point getCurTimePoint(void)
{
    return std::chrono::steady_clock::now();
}

inline std::chrono::steady_clock::time_point getAfterTimePoint(float time)
{
    return std::chrono::steady_clock::now() + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(time/1000));
}

inline float getMilisecondTime(const std::chrono::steady_clock::time_point& curTime, const std::chrono::steady_clock::time_point& _prevTime)
{
    std::chrono::milliseconds millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - _prevTime);
    return millisecond.count();
}