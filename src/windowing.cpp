#include "windowing.h"

#include <iostream>
#include "config.h"

Program program;

void setCallbacks();

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const char * message, const void * userParam);

void setActiveProgram(GLuint id) {
    program.activeId = id;
    glUseProgram(program.activeId);
}

// GLFW and OpenGL program initialization function
bool init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // debug context
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

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

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    // tell OpenGL to render its stuff inside the GLFW window
    glViewport(0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    // set resizing, input, etc. callbacks
    setCallbacks();

    gl::programId = glCreateProgram();
    gl::lightingId = glCreateProgram();
//    setActiveProgram(gl::programId);

    // create and set active camera
    Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    program.activeCamera = mainCamera;

    glEnable(GL_DEPTH_TEST);

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
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        Config::ENABLE_DEBUG = !Config::ENABLE_DEBUG;

//    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
//        std::cout << "Camera values:\tYaw = " << program.activeCamera.mYaw << " |\tPitch = " << program.activeCamera.mPitch << std::endl;
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

void keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if (mods & GLFW_MOD_SHIFT)
        program.activeCamera.mIsSprinting = true;
    else
        program.activeCamera.mIsSprinting = false;
}

void setCallbacks() {
    glfwSetFramebufferSizeCallback(gl::mainWindow, framebufferSizeCallback);
    glfwSetCursorPosCallback(gl::mainWindow, mouseCallback);
    glfwSetScrollCallback(gl::mainWindow, scrollCallback);
    glfwSetKeyCallback(gl::mainWindow, keyboardCallback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(gl::mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const char * message, const void * userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;
    std::cout << "---------" << '\n';
    std::cout << "Debug message (" << id << "): " << message << '\n';

    switch (source) {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    }
    std::cout << '\n';

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << '\n';

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << '\n';

    std::cout << std::endl;
}