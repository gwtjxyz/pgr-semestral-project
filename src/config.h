#pragma once

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <nanogui/nanogui.h>

#include "objects/materials.h"

#define STB_IMAGE_IMPLEMENTATION

// TODO add fancy description

// namespace storing pre-defined configuration constants
// this generally doesn't change during the program's run
// unless directly requested by the user
// TODO add this to options menu or something
namespace Config {
    extern int WINDOW_WIDTH ;
    extern int WINDOW_HEIGHT;

    extern bool ENABLE_DEBUG;
    extern bool ENABLE_FULLSCREEN;

    extern float FOV;          // vertical FOV - equal to 90° horizontal FOV
    extern float ZNEAR;
    extern float ZFAR;

    // camera defaults
    const float YAW =   -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;
    const float SCENE_BOUNDS = 50.0f;

    // Perlin noise - for terrain generation
    const float Z_CONSTANT = 0.8f;
    const int NOISE_INTENSITY = 10;
    const float Y_BOTTOM = -4.0f;               // lowest possible y - at result 0
    const float Y_RANGE = 8.0f;                 // should be -2 * Y_BOTTOM

    // terrain
    const int TERRAIN_SIZE = 512;
    const int TERRAIN_TEXTURE_STEP = 32;
    const char * const TERRAIN_TEXTURE_DIFFUSE = R"(../resources/deadgrass.jpg)";
    const char * const TERRAIN_TEXTURE_SPECULAR = R"(../resources/deadgrass.jpg)";
    const float TERRAIN_Y = -6.0f;

    // models
    const std::string TEXTURE_DIFFUSE = "diffuse";
    const std::string TEXTURE_SPECULAR = "specular";

    // static camera settings
    const glm::vec3 STATIC_CAMERA_POS_1 = glm::vec3(-20.0f, 8.0f, -5.0f);
    const float STATIC_CAMERA_YAW_1 = 30.0f;
    const float STATIC_CAMERA_PITCH_1 = -10.0f;

    const glm::vec3 STATIC_CAMERA_POS_2 = glm::vec3(9.6f, -1.8f, -4.4f);
    const float STATIC_CAMERA_YAW_2 = -161.0f;
    const float STATIC_CAMERA_PITCH_2 = -15.7f;
}

// namespace storing rendering-related variables
// this changes dynamically during the program's execution
namespace gl {
    extern nanogui::Screen * screen;
    extern nanogui::FormHelper * gui;
    extern bool guiEnabled;
    extern GLuint pickFBO;
    extern GLuint programId;
    extern GLuint lightingId;
    extern GLuint skyboxId;
    extern GLuint logoId;
    extern GLuint pickObjectId;
    extern GLuint fireId;
    extern GLFWwindow * mainWindow;
    extern float deltaTime;
    extern float lastFrame;
    extern bool firstMouse;
    extern float lastX;
    extern float lastY;
    extern bool logoEnabled;
    extern float logoTime;
    extern float fireTime;
    extern bool enableMovement;
    // pick object IDs
    extern int swordId;
    extern int towerId;
    extern int fireplaceId;
    // pick object states
    extern bool swordClicked;
    extern bool towerClicked;
    extern bool fireplaceClicked;
}