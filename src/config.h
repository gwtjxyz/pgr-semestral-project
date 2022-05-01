#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

// TODO add fancy description

// namespace storing pre-defined configuration constants
// this generally doesn't change during the program's run
// unless directly requested by the user
// TODO add this to options menu or something
namespace Config {
    extern int WINDOW_WIDTH ;
    extern int WINDOW_HEIGHT;

    extern bool ENABLE_DEBUG;

    extern float FOV;          // vertical FOV - equal to 90° horizontal FOV
    extern float ZNEAR;
    extern float ZFAR;

    // camera defaults
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.03f;
    const float ZOOM = 45.0f;
}

// namespace storing rendering-related variables
// this changes dynamically during the program's execution
namespace gl {
    extern GLuint programId;
    extern GLFWwindow * mainWindow;
    extern float deltaTime;
    extern float lastFrame;
    extern bool firstMouse;
    extern float lastX;
    extern float lastY;
}