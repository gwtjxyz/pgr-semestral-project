#pragma once

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

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
    const char * const TERRAIN_TEXTURE_DIFFUSE = R"(../resources/grass.jpg)";
    const char * const TERRAIN_TEXTURE_SPECULAR = R"(../resources/grass.jpg)";
    const float TERRAIN_Y = -6.0f;

    // models
    const std::string TEXTURE_DIFFUSE = "diffuse";
    const std::string TEXTURE_SPECULAR = "specular";
}

// namespace storing rendering-related variables
// this changes dynamically during the program's execution
namespace gl {
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