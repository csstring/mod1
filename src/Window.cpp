#include "Window.h"
#include "Simulator.h"
#include "Camera.h"
#include "EnumHeader.h"
#include "Scene.h"
#include "ParticleScene.h"

void Window::initialize(void)
{
    if (glfwInit() == false) 
        ft_assert("glfwInit false");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    _window = glfwCreateWindow(WINDOW_WITH, WINDOW_HEIGHT, "model", NULL, NULL);  
    if (_window == NULL) 
    {
        glfwTerminate();
        ft_assert("glfw create Window failed");
    }
    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwMakeContextCurrent(_window);
    if (glewInit() != GLEW_OK) 
        ft_assert("glew init failed");

    clearColorSetUp();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_CULL_FACE);
    glPointSize(4);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); 
    glDepthFunc(GL_LESS);
}

void Window::processInput(float delta, Simulator& simul)
{
    Camera& camera = Camera::getInstance();
    static int currentMouseState,previousMouseState;
    static int currentGravityState,previousGravityState;
    static int currentSpeed,previousSpeed;
    static int SpeedCash = 0;
    currentMouseState = glfwGetKey(_window, GLFW_KEY_SPACE);
    currentGravityState = glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT);
    currentSpeed = glfwGetKey(_window, GLFW_KEY_P);

    if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, GLFW_TRUE);

    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera._cameraPos += delta* camera._movementSpeed * camera._cameraFront;
    }
    else if (glfwGetKey(_window, GLFW_KEY_S ) == GLFW_PRESS)
    {
        camera._cameraPos -= delta* camera._movementSpeed * camera._cameraFront;
    }
    else if (glfwGetKey(_window, GLFW_KEY_A ) == GLFW_PRESS)
    {
        camera._cameraPos -= camera._cameraRight * delta* camera._movementSpeed;
    }
    else if (glfwGetKey(_window, GLFW_KEY_D ) == GLFW_PRESS)
    {
        camera._cameraPos += camera._cameraRight * delta* camera._movementSpeed;
    }

    if (currentMouseState == GLFW_RELEASE && previousMouseState == GLFW_PRESS) {
        previousMouseState = currentMouseState;
        camera._isOn = camera._isOn == true ? false : true;
    } else if (currentMouseState == GLFW_PRESS && previousMouseState == GLFW_RELEASE){
        previousMouseState = currentMouseState;
    }

    if (currentSpeed == GLFW_RELEASE && previousSpeed == GLFW_PRESS) {
        previousSpeed = currentSpeed;
        std::swap(SpeedCash, simul._speed);
    } else if (currentSpeed == GLFW_PRESS && previousSpeed == GLFW_RELEASE){
        previousSpeed = currentSpeed;
    }

    if (currentGravityState == GLFW_RELEASE && previousGravityState == GLFW_PRESS) {
        previousGravityState = currentGravityState;
        ParticleScene* ps = static_cast<ParticleScene*>(simul._particleScene.get());
        ps->_isGravityOn = ps->_isGravityOn == true ? false : true;
    } else if (currentGravityState == GLFW_PRESS && previousGravityState == GLFW_RELEASE){
        previousGravityState = currentGravityState;
    }
}

void Window::clearColorSetUp(float r, float g, float b, float a)
{
    glClearColor(r,g,b,a);
}

bool Window::isWindowClose(void)
{
    if (glfwWindowShouldClose(_window) == 0)
        return false;
    return true;
}

void Window::bufferSwap(void)
{
    glfwSwapBuffers(_window);
}