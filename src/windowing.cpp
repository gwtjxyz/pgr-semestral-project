#include "windowing.h"

#include <iostream>
#include "config.h"
#include "program.h"
#include "render.h"
#include "utils.h"

void setCallbacks();

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const char * message, const void * userParam);

void initNanogui();

void switchGui();

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
    gl::skyboxId = glCreateProgram();
    gl::logoId = glCreateProgram();
    gl::pickObjectId = glCreateProgram();
    gl::fireId = glCreateProgram();

    // create and set active camera
    Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    program.mainCamera = mainCamera;
    program.activeCamera = &program.mainCamera;
    // static cameras
    Camera static1(Config::STATIC_CAMERA_POS_1,
                   {0.0f, 1.0f, 0.0f},
                   Config::STATIC_CAMERA_YAW_1,
                   Config::STATIC_CAMERA_PITCH_1);
    static1.mStatic = true;
    program.staticCamera1 = static1;

    Camera static2(Config::STATIC_CAMERA_POS_2,
                   {0.0f, 1.0f, 0.0f},
                   Config::STATIC_CAMERA_YAW_2,
                   Config::STATIC_CAMERA_PITCH_2);
    static2.mStatic = true;
    program.staticCamera2 = static2;
    // bird POV
    Camera dynamic({-15.0f, 14.0f, -15.0f},
                   {0.0f, 1.0f, 0.0f},
                   Config::YAW,
                   -30.0f);
    dynamic.mStatic = true;
    program.dynamicCamera = dynamic;

    // UI
    initNanogui();

    glEnable(GL_DEPTH_TEST);

    return true;
}

void initNanogui() {
    gl::screen = new nanogui::Screen();
    gl::screen->initialize(gl::mainWindow, true);

    // create nanogui gui
    gl::gui = new nanogui::FormHelper(gl::screen);
    nanogui::ref<nanogui::Window> menuWindow1 =
            gl::gui->addWindow(Eigen::Vector2i(10, 10), "Interactibles and cameras");
    gl::gui->addGroup("Interactive objects");
    gl::gui->addButton("Tower", []() { gl::towerClicked = !gl::towerClicked; })->setTooltip("Gives the tower an ominous glow.");
    gl::gui->addButton("Sword", []() { gl::swordClicked = !gl::swordClicked; })->setTooltip("Spins the sword around. Weeeeeeeeee!");
    gl::gui->addButton("Campfire", []() { gl::fireplaceClicked = !gl::fireplaceClicked; })->setTooltip("Ignites the campfire (or snuffs it out)");

    gl::gui->addGroup("Cameras");
    gl::gui->addButton("Main camera", [] () { program.activeCamera = &program.mainCamera; })->setTooltip("Default hotkey: 1");
    gl::gui->addButton("Static camera 1", [] () { program.activeCamera = &program.staticCamera1; })->setTooltip("Default hotkey: 2");
    gl::gui->addButton("Static camera 2", [] () { program.activeCamera = &program.staticCamera2; })->setTooltip("Default hotkey: 3");
    gl::gui->addButton("Bird camera", [] () { program.activeCamera = &program.dynamicCamera; })->setTooltip("Default hotkey: 4");

    gl::screen->setVisible(true);

    nanogui::ref<nanogui::Window> menuWindow2 =
            gl::gui->addWindow( Eigen::Vector2i(10, 10), "Additional controls");
    gl::gui->addGroup("Main menu");
    gl::gui->addButton("Back", []() { switchGui(); })->setTooltip("Closes this menu.");
    gl::gui->addButton("Exit", []() { glfwSetWindowShouldClose(gl::mainWindow, true); })->setBackgroundColor({200, 20, 20, 200});

    gl::gui->addGroup("Misc");
    gl::gui->addButton("Flashlight", []() { program.enableSpotlight = !program.enableSpotlight; })->setTooltip("Default hotkey: E");
    gl::gui->addButton("Scrolling logo", []() { gl::logoEnabled = !gl::logoEnabled; })->setTooltip("Default hotkey: F1");
    gl::gui->addButton("Normal pills", []() { program.enableWarp = !program.enableWarp; })->setTooltip("Default hotkey: Q");
    gl::gui->addButton("Show wireframes", []() { Config::ENABLE_DEBUG = !Config::ENABLE_DEBUG; })->setTooltip("Default hotkey: F");

    gl::screen->performLayout();
    menuWindow1->setPosition(Eigen::Vector2i(Config::WINDOW_WIDTH / 4, (Config::WINDOW_HEIGHT / 4)));
    menuWindow2->setPosition(Eigen::Vector2i(Config::WINDOW_WIDTH / 2, (Config::WINDOW_HEIGHT / 4)));
}

void switchGui() {
    if (gl::guiEnabled)
        glfwSetInputMode(gl::mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(gl::mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    gl::guiEnabled = !gl::guiEnabled;
    program.mainCamera.mStatic = !program.mainCamera.mStatic;
}

void framebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
}

// TODO
void switchResolutions() {
//    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
//    if ()
}

/// For processing repeating input - hence why movement is in here instead of the other callback
/// @param[in] window       GLFW window pointer
void processInput(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        program.activeCamera->processKeyboard(CameraDirections::FORWARD, gl::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        program.activeCamera->processKeyboard(CameraDirections::BACKWARD, gl::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        program.activeCamera->processKeyboard(CameraDirections::LEFT, gl::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        program.activeCamera->processKeyboard(CameraDirections::RIGHT, gl::deltaTime);

    switchResolutions();
}

void pickObject(int button) {
    // prevent conflicting input with popup GUI
    if (gl::guiEnabled)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, gl::pickFBO);
    unsigned char pixel[4];
    // doesn't use shaders - allows to read RGBA values directly
    glReadPixels(Config::WINDOW_WIDTH / 2,
                 Config::WINDOW_HEIGHT / 2,
                 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    if (pixel[1] == 0) {
        std::cout << "Clicked on nothing\n";
    } else {
        std::cout << "Clicked on object " << (int) pixel[0] << std::endl;
        if (pixel[0] == gl::swordId) {
            gl::swordClicked = !gl::swordClicked;
        } else if (pixel[0] == gl::towerId) {
            gl::towerClicked = !gl::towerClicked;
        } else if (pixel[0] == gl::fireplaceId) {
            gl::fireplaceClicked = !gl::fireplaceClicked;
            gl::fireTime = 0.0f;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    program.activeCamera->processMouseMovement(xOffset, yOffset);
    gl::screen->cursorPosCallbackEvent(xPosIn, yPosIn);
}

void scrollCallback(GLFWwindow * window, double xOffset, double yOffset) {
    program.activeCamera->processMouseScroll((float) yOffset);
}

void keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if (mods & GLFW_MOD_SHIFT)
        program.activeCamera->mIsSprinting = true;
    else
        program.activeCamera->mIsSprinting = false;

    if (mods & GLFW_MOD_ALT)
        glfwSetInputMode(gl::mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        Config::ENABLE_DEBUG = !Config::ENABLE_DEBUG;
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        program.enableWarp = !program.enableWarp;
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        program.enableSpotlight = !program.enableSpotlight;

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        gl::logoEnabled = !gl::logoEnabled;
        if (gl::logoEnabled)
            gl::logoTime = 0.0f;    // reset every time we turn it on
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        program.activeCamera = &program.mainCamera;
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        program.activeCamera = &program.staticCamera1;
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        program.activeCamera = &program.staticCamera2;
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        program.activeCamera = &program.dynamicCamera;

    // NanoGUI
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        switchGui();
    }
}

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !gl::guiEnabled) {
        glfwSetInputMode(gl::mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        pickObject(button);
    }
    gl::screen->mouseButtonCallbackEvent(button, action, mods);
}

void setCallbacks() {
    glfwSetFramebufferSizeCallback(gl::mainWindow, framebufferSizeCallback);
    glfwSetCursorPosCallback(gl::mainWindow, mouseCallback);
    glfwSetScrollCallback(gl::mainWindow, scrollCallback);
    glfwSetKeyCallback(gl::mainWindow, keyboardCallback);
    glfwSetMouseButtonCallback(gl::mainWindow, mouseButtonCallback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(gl::mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
/// see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDebugMessageCallback.xhtml
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