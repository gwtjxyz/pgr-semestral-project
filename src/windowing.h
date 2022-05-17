#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       windowing.h
 * \author     Yury Udavichenka
 * \date       18/04/2022
 * \brief      Window handling
 *
 * Contains code for setting up the initial window context,
 * GUI, as well as processing input and various callbacks related to it.
 *
*/
//----------------------------------------------------------------------------------------
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "camera.h"

/// sets up everything
bool init();

void framebufferSizeCallback(GLFWwindow * window, int width, int height);

void processInput(GLFWwindow * window);

void pickObject(int button);

void mouseCallback(GLFWwindow * window, double xPosIn, double yPosIn);

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);

void scrollCallback(GLFWwindow * window, double xOffset, double yOffset);

void keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods);