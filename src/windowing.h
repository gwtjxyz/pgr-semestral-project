#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

// TODO add fancy description

bool init();

void framebufferSizeCallback(GLFWwindow * window, int width, int height);

void processInput(GLFWwindow * window);