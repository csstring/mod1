#pragma once
#include "Common.h"

constexpr float cameraSpeed = 0.5f;
class Camera
{

    public:
        ~Camera(){};
        void update(void);
        void initialize(void);
        void updateCameraVectors(void);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);
        glm::vec4 getWorldCursorPos() const;
        const glm::mat4& getProjection() const {return projection;};
        static Camera& getInstance() {
          static Camera instance; // Meyers' Singleton
          return instance;
        }

    private:
        Camera(){};
        float _yaw;
        float _pitch;
        float _mouseSensitivity;
        glm::mat4 projection;

    public:
        float _movementSpeed;
        float _fov;
        bool _isFirst;
        bool _isOn;
        float _lastX;
        float _lastY;
        float _zNear = 0.1;
        float _zFar = 100;
        glm::vec3   _worldUp;
        glm::vec3   _cameraUp;
        glm::vec3   _cameraPos;
        glm::vec3   _cameraFront;
        glm::vec3   _cameraRight;
        glm::mat4   _view;
};  