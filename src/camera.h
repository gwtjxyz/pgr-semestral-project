#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       camera.h
 * \author     Yury Udavichenka
 * \date       30/04/2022
 * \brief      A brief file description
 *
 *  A more detailed file description.
 *
*/
//----------------------------------------------------------------------------------------
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "config.h"

enum class CameraDirections {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// 3D camera with controls
class Camera {
public:
    // vector constructor
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = Config::YAW,
           float pitch = Config::PITCH);
    // scalar constructor
    Camera(float posX, float posY, float posZ,
           float upX, float upY, float upZ,
           float yaw, float pitch);

    ~Camera() = default;

    glm::mat4 getViewMatrix();

    glm::mat4 getQuatViewMatrix();

    void processKeyboard(CameraDirections direction, float deltaTime);

    void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

    void processMouseScroll(float yOffset);

private:
    // calculates front vector from Euler angles
    void updateCameraVectors();
public:
    // camera attribs
    glm::vec3 mPosition{};
    glm::vec3 mFront{};
    glm::vec3 mUp{};
    glm::vec3 mRight{};
    glm::vec3 mWorldUp{};
    // Euler angles
    float mYaw;
    float mPitch;
    // camera options
    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;
};



