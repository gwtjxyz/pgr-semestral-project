#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       camera.h
 * \author     Yury Udavichenka
 * \date       30/04/2022
 * \brief      Camera class
 *
 *  Header file containing the camera class used for both dynamic and static angles
 *  and calculating the viewport
 *
*/
//----------------------------------------------------------------------------------------
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "config.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraDirections {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    /// constructor with vectors
    /**
     * Creates a new camera with given parameters
     * @param[in] position      camera's starting position
     * @param[in] up            world up vector
     * @param[in] yaw           starting yaw value
     * @param[in] pitch         starting pitch value
     */
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = Config::YAW, float pitch = Config::PITCH);
    /// constructor with scalar values
    /**
     * Creates a new camera with given parameters
     * @param[in] posX          Camera's starting X position
     * @param[in] posY          Camera's starting Y position
     * @param[in] posZ          Camera's starting Z position
     * @param[in] upX           World up vector's X coordinate
     * @param[in] upY           World up vector's Y coordinate
     * @param[in] upZ           World up vector's Z coordinate
     * @param[in] yaw           starting yaw value
     * @param[in] pitch         starting pitch value
     */
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    /// returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix();
    /// processes input received from any keyboard-like input system.
    /**
     * @param[in] direction         enum specifying move direction
     * @param[in] deltaTime         time delta for calculating how far to move
     */
    void processKeyboard(CameraDirections direction, float deltaTime);

    /// processes input received from a mouse input system.
    /**
     * @param[in] xOffset           mouse X offset
     * @param[in] yOffset           mouse Y offset
     * @param[in] constrainPitch    pitch constraint
     */
    void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

    /// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yOffset);

    /// calculates the front vector from the Camera's (updated) Euler Angles.
    /// This stopped working at some point (I don't know why)
    void updateCameraVectors();
private:


public:
    // camera Attributes
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp;
    // euler Angles
    float mYaw;
    float mPitch;
    // camera options
    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;
    bool mIsSprinting;
    bool mStatic;
};