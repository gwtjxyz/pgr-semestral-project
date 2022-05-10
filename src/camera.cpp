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
#include <iostream>

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
    mIsSprinting = false;
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
    mIsSprinting = false;
    updateCameraVectors();
}



glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::processKeyboard(CameraDirections direction, float deltaTime) {
    float velocity;
    if (mIsSprinting)
        velocity = mMovementSpeed * 4 * deltaTime;
    else
        velocity = mMovementSpeed * deltaTime;

    if (direction == CameraDirections::FORWARD)
        mPosition += mFront * velocity;
    if (direction == CameraDirections::BACKWARD)
        mPosition -= mFront * velocity;
    if (direction == CameraDirections::LEFT)
        mPosition -= mRight * velocity;
    if (direction == CameraDirections::RIGHT)
        mPosition += mRight * velocity;

    // space constraints
    if (mPosition.y <= Config::TERRAIN_Y + 1.0f)
        mPosition.y = Config::TERRAIN_Y + 1.0f;
    if (mPosition.y >= Config::SCENE_BOUNDS)
        mPosition.y = Config::SCENE_BOUNDS;


    if (mPosition.x >= Config::SCENE_BOUNDS)
        mPosition.x = Config::SCENE_BOUNDS;
    if (mPosition.x <= -Config::SCENE_BOUNDS)
        mPosition.x = -Config::SCENE_BOUNDS;

    if (mPosition.z >= Config::SCENE_BOUNDS)
        mPosition.z = Config::SCENE_BOUNDS;
    if (mPosition.z <= -Config::SCENE_BOUNDS)
        mPosition.z = -Config::SCENE_BOUNDS;

}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch) {
    xOffset *= mMouseSensitivity;
    yOffset *= mMouseSensitivity;

//    mYaw = glm::mod(mYaw + (float) xOffset, 360.0f);
    mYaw   += xOffset;
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
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(front);
//    // recalculate right and up vectors
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp    = glm::normalize(glm::cross(mRight, mFront));
}

