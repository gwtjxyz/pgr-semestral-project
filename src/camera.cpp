//----------------------------------------------------------------------------------------
/**
 * \file       camera.cpp
 * \author     Yury Udavichenka
 * \date       30/04/2022
 * \brief      A brief file description
 *
 *  A more detailed file description.
 *
*/
//----------------------------------------------------------------------------------------
#include "camera.h"
#include <glm/detail/type_quat.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

// TODO maybe make this into a quaternion camera so it actually works properly
Camera::Camera(glm::vec3 position,
               glm::vec3 up,
               float yaw,
               float pitch) {
    mPosition = position;
    mFront = glm::vec3(0.0f, 0.0f, -1.0f);
    mWorldUp = up;
    mYaw = yaw;
    mPitch = pitch;
    mMovementSpeed = Config::SPEED;
    mMouseSensitivity = Config::SENSITIVITY;
    mZoom = Config::ZOOM;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ,
               float upX, float upY, float upZ,
               float yaw, float pitch) {
    mPosition = glm::vec3(posX, posY, posZ);
    mFront = glm::vec3(0.0f, 0.0f, -1.0f);
    mWorldUp = glm::vec3(upX, upY, upZ);
    mYaw = yaw;
    mPitch = pitch;
    mMovementSpeed = Config::SPEED;
    mMouseSensitivity = Config::SENSITIVITY;
    mZoom = Config::ZOOM;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}
// TODO fix this broken mess
glm::mat4 Camera::getQuatViewMatrix() {
    glm::mat4 viewMatrix;
    glm::quat qPitch = glm::angleAxis(mPitch, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat qYaw = glm::angleAxis(mYaw, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::quat orientation = qPitch * qYaw;
    orientation = glm::normalize(orientation);
    glm::mat4 rotate = glm::mat4_cast(orientation);

    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, -mPosition);

    viewMatrix = rotate * translate;
    return viewMatrix;
}


void Camera::processKeyboard(CameraDirections direction, float deltaTime) {
    float velocity = mMovementSpeed * deltaTime;

    switch (direction) {
        case CameraDirections::FORWARD:
            mPosition += mFront * velocity;
            break;
        case CameraDirections::BACKWARD:
            mPosition -= mFront * velocity;
            break;
        case CameraDirections::LEFT:
            mPosition -= mRight * velocity;
            break;
        case CameraDirections::RIGHT:
            mPosition += mRight * velocity;
            break;
    }
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch) {
    xOffset *= mMouseSensitivity;
    yOffset *= mMouseSensitivity;

    mYaw = glm::mod(mYaw + xOffset, 360.0f);
    mPitch += yOffset;

    // don't flip the screen if pitch is out of bounds
    if (constrainPitch) {
        if (mPitch > 89.0f)
            mPitch = 89.0f;
        if (mPitch < -89.0f)
            mPitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset) {
    mZoom -= (float) yOffset;
    if (mZoom < 1.0f)
        mZoom = 1.0f;
    if (mZoom > 45.0f)
        mZoom = 45.0f;
}

void Camera::updateCameraVectors() {
    // pitch = Y axis, yaw = X axis
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw) * cos(glm::radians(mPitch)));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw) * cos(glm::radians(mPitch)));
    mFront = glm::normalize(front);
    // recalculate right and up vectors
    mRight = glm::normalize(glm::cross(mWorldUp, mFront));
    mUp    = glm::normalize(glm::cross(mRight, mFront));
}

