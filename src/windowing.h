#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "camera.h"

// TODO add fancy description

struct Program {
    Camera activeCamera;
};

extern Program program;

bool init();

void framebufferSizeCallback(GLFWwindow * window, int width, int height);

void processInput(GLFWwindow * window);

void mouseCallback(GLFWwindow * window, double xPosIn, double yPosIn);

void scrollCallback(GLFWwindow * window, double xOffset, double yOffset);