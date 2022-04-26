#include <iostream>
#include "windowing.h"
#include "config.h"

// GLFW and OpenGL program initialization function
bool init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    gl::mainWindow = glfwCreateWindow(Constants::WINDOW_WIDTH,
                              Constants::WINDOW_HEIGHT,
                              "Evil Tower",
                              nullptr,
                              nullptr);
    if (gl::mainWindow == nullptr) {
        std::cerr << "Failed to create GLFW window!\n";
        return false;
    }
    glfwMakeContextCurrent(gl::mainWindow);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!\n";
        return false;
    }

    // tell OpenGL to render its stuff inside the GLFW window
    glViewport(0, 0, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
    // tell GLFW what to use for resizing
    glfwSetFramebufferSizeCallback(gl::mainWindow, framebufferSizeCallback);

    gl::programId = glCreateProgram();
    return true;
}

void framebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window) {
    // TODO add shader reload functionality
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

