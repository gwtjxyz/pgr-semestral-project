#include "windowing.h"

#include <iostream>
#include "config.h"

Program program;

void setCallbacks();

// GLFW and OpenGL program initialization function
bool init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    gl::mainWindow = glfwCreateWindow(Config::WINDOW_WIDTH,
                                      Config::WINDOW_HEIGHT,
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
    glViewport(0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    // set resizing, input, etc. callbacks
    setCallbacks();

    gl::programId = glCreateProgram();

    // create and set active camera
    Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    program.activeCamera = mainCamera;

    return true;
}

void framebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        program.activeCamera.processKeyboard(CameraDirections::FORWARD, gl::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        program.activeCamera.processKeyboard(CameraDirections::BACKWARD, gl::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        program.activeCamera.processKeyboard(CameraDirections::LEFT, gl::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        program.activeCamera.processKeyboard(CameraDirections::RIGHT, gl::deltaTime);

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        std::cout << "Camera values:\tYaw = " << program.activeCamera.mYaw << " |\tPitch = " << program.activeCamera.mPitch << std::endl;
}

void mouseCallback(GLFWwindow * window, double xPosIn, double yPosIn) {
    float xPos = (float) xPosIn;
    float yPos = (float) yPosIn;

    if (gl::firstMouse) {
        gl::lastX = xPos;
        gl::lastY = yPos;
        gl::firstMouse = false;
    }

    float xOffset = xPos - gl::lastX;
    float yOffset = gl::lastY - yPos;   // reversed since y-coords go from bottom to top

    gl::lastX = xPos;
    gl::lastY = yPos;

    program.activeCamera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow * window, double xOffset, double yOffset) {
    program.activeCamera.processMouseScroll((float) yOffset);
}

void setCallbacks() {
    glfwSetFramebufferSizeCallback(gl::mainWindow, framebufferSizeCallback);
    glfwSetCursorPosCallback(gl::mainWindow, mouseCallback);
    glfwSetScrollCallback(gl::mainWindow, scrollCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(gl::mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}