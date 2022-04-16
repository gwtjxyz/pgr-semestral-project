#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

// TODO add fancy description

// namespace storing configuration constants
namespace Constants {
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
}

// namespace storing rendering-related variables
namespace gl {
    extern GLuint programId;
    extern GLFWwindow * mainWindow;
}