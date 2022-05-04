#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "camera.h"

// TODO add fancy description
// TODO rename this file, this name is horrible

struct Program {
    Camera activeCamera;
    GLuint activeId;
};

extern Program program;

void setActiveProgram(GLuint program);

bool init();

void framebufferSizeCallback(GLFWwindow * window, int width, int height);

void processInput(GLFWwindow * window);

void mouseCallback(GLFWwindow * window, double xPosIn, double yPosIn);

void scrollCallback(GLFWwindow * window, double xOffset, double yOffset);

void keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods);