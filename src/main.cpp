#define GL_SILENCE_DEPRECATION
#include "Window.h"
#include "shader.h"
#include "Simulator.h"
#include "Camera.h"
#include <iostream>
#include "Mygui.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGuiIO& io = ImGui::GetIO();

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        io.MouseDown[0] = action == GLFW_PRESS;
    }

    // Check if IMGUI wants to capture the mouse
    if (!io.WantCaptureMouse) {
        Camera& _camera = Camera::getInstance();
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            _camera._clickOn = true;
        } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
            _camera._clickOn = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    (void)window;
    Camera& _camera = Camera::getInstance();
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(xposIn, yposIn);
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (_camera._isFirst)
    {
        _camera._lastX = xpos;
        _camera._lastY = ypos;
        _camera._isFirst = false;
    }

    float xoffset = xpos - _camera._lastX;
    float yoffset = _camera._lastY - ypos;

    _camera._lastX = xpos;
    _camera._lastY = ypos;

    if (_camera._isOn == false)
        return ;
    _camera.ProcessMouseMovement(xoffset, yoffset, true);
}
void cvcheck() {
    cl_uint numPlatforms;
    clGetPlatformIDs(0, NULL, &numPlatforms);
    cl_platform_id* platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * numPlatforms);
    clGetPlatformIDs(numPlatforms, platforms, NULL);

    for (cl_uint i = 0; i < numPlatforms; ++i) {
        char info[1024];
        clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 1024, info, NULL);
        std::cout << "Platform Name: " << info << std::endl;

        clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 1024, info, NULL);
        std::cout << "Platform Vendor: " << info << std::endl;

        clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 1024, info, NULL);
        std::cout << "Platform Version: " << info << std::endl;

        clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 1024, info, NULL);
        std::cout << "Platform Extensions: " << info << std::endl;
        if (strstr(info, "cl_khr_3d_image_writes") != NULL) {
            std::cout << "\tcl_khr_3d_image_writes supported" << std::endl;
        } else {
            std::cout << "\tcl_khr_3d_image_writes not supported" << std::endl;
        }

        cl_uint numDevices;
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
        cl_device_id* devices = (cl_device_id*)malloc(sizeof(cl_device_id) * numDevices);
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);

        for (cl_uint j = 0; j < numDevices; ++j) {
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(info), info, NULL);
            std::cout << "\tDevice Name: " << info << std::endl;

            size_t maxWorkGroupSize;
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(maxWorkGroupSize), &maxWorkGroupSize, NULL);
            std::cout << "\tMax Work Group Size: " << maxWorkGroupSize << std::endl;
            cl_bool imageSupport;
        clGetDeviceInfo(devices[j], CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool), &imageSupport, NULL);
        std::cout << "\tImage Support: " << (imageSupport ? "Yes" : "No") << std::endl;

        char deviceExtensions[2048];
        clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, sizeof(deviceExtensions), deviceExtensions, NULL);
        std::cout << "\tDevice Extensions: " << deviceExtensions << std::endl;
        if (strstr(deviceExtensions, "cl_khr_3d_image_writes") != NULL) {
            std::cout << "\t\tcl_khr_3d_image_writes supported" << std::endl;
        } else {
            std::cout << "\t\tcl_khr_3d_image_writes not supported" << std::endl;
        }
    }

    free(devices);
    }

    free(platforms);
}

int main(int ac, char** av) 
{
    if (ac < 2){
        std::cerr << "input error : particle max count missing\n";
        return 1;
    } else if (ac > 2) {
        std::cerr << "input error : to many argument\n";
        return 1;
    }
    cvcheck();
    float count = std::atof(av[1]);
    if (count == 0.0f){
        std::cerr << "input error : not number";
        return 1;
    }

    Window      window;
    Simulator*  simulator = new Simulator();
    Camera& _camera = Camera::getInstance();
    window.initialize();
    _camera.initialize();
    Mygui::getInstance().initialize(window._window);
    simulator->initialize(std::ceil(count / 64.0f) * 64.0f);

    glfwSetFramebufferSizeCallback(window._window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window._window, mouse_callback);
    glfwSetInputMode(window._window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window._window, mouse_button_callback);
    constexpr float delta = 1.0f / 60.0f;

    while (window.isWindowClose() == false)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Mygui::getInstance().update(*simulator);
        window.processInput(delta, *simulator);
        _camera.update();
        simulator->update(delta * simulator->_speed);
        simulator->draw();
        Mygui::getInstance().render();
        window.bufferSwap();
        glfwPollEvents();
    }
    delete simulator;
    glfwDestroyWindow(window._window);
    glfwTerminate();
}