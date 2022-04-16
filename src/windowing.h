#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

// TODO add fancy description

bool init();

inline void framebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
}

inline void processInput(GLFWwindow * window) {
    // TODO add shader reload functionality
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}