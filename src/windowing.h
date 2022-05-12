#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "camera.h"

// TODO add fancy description
// TODO rename this file, this name is horrible

bool init();

void framebufferSizeCallback(GLFWwindow * window, int width, int height);

void processInput(GLFWwindow * window);

void pickObject(int button);

void mouseCallback(GLFWwindow * window, double xPosIn, double yPosIn);

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);

void scrollCallback(GLFWwindow * window, double xOffset, double yOffset);

void keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods);