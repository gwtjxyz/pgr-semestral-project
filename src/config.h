#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       config.h
 * \author     Yury Udavichenka
 * \date       30/04/2022
 * \brief      Configuration and globals header file
 *
 *  Header containing global constants that (generally speaking) don't change
 *  as well as certain global variables that can (and do) change over the
 *  course of the application's runtime.
 *
*/
//----------------------------------------------------------------------------------------
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <nanogui/nanogui.h>

#include "objects/materials.h"

#define STB_IMAGE_IMPLEMENTATION

/// namespace storing pre-defined configuration constants\n
/// this generally doesn't change during the program's run\n
/// unless directly requested by the user
namespace Config {
    extern int WINDOW_WIDTH;
    extern int WINDOW_HEIGHT;

    extern bool ENABLE_DEBUG;       ///< Debug parameter - currently used for drawing wireframes
    extern bool ENABLE_FULLSCREEN;  ///< UNUSED

    extern float FOV;          ///< vertical FOV - equal to approx. 90° horizontal FOV
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
    const float Z_CONSTANT = 0.8f;              ///< Constant value for noise generator's Z input
    const int NOISE_INTENSITY = 10;             ///< Constant specifying noise intensity - the higher the value, the bumpier the resulting terrain
    const float Y_BOTTOM = -4.0f;               ///< Constant specifying minimum generated height - UNUSED
    const float Y_RANGE = 8.0f;                 ///< Constant specifying range of generated heights - UNUSED

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

/// namespace storing rendering-related variables\n
/// this changes dynamically during the program's execution
namespace gl {
    extern nanogui::Screen * screen;                ///< Nanogui screen variable
    extern nanogui::FormHelper * gui;               ///< Nanogui GUI container
    extern bool guiEnabled;                         ///< controls whether or not to draw the GUI
    extern GLuint pickFBO;                          ///< Picking framebuffer object
    extern GLuint programId;                        ///< Main shader program
    extern GLuint lightingId;                       ///< Lighting cube shader program
    extern GLuint skyboxId;                         ///< Skybox shader program
    extern GLuint logoId;                           ///< OpenGL logo shader program
    extern GLuint pickObjectId;                     ///< Pick object shader program
    extern GLuint fireId;                           ///< Fire animation texture program
    extern GLFWwindow * mainWindow;                 ///< GLFW window
    extern float deltaTime;                         ///< Time delta
    extern float lastFrame;                         ///< Last frame timestamp
    extern bool firstMouse;                         ///< Whether or not the mouse input was the first yet - for camera
    extern float lastX;                             ///< Mouse's last X pos
    extern float lastY;                             ///< Mouse's last Y pos
    extern bool logoEnabled;                        ///< Controls OpenGL logo drawing
    extern float logoTime;                          ///< Controls OpenGL logo scrolling
    extern float fireTime;                          ///< Controls fire texture animation
    // pick object IDs
    extern int swordId;                             ///< Sword pick object ID
    extern int towerId;                             ///< Tower pick object ID
    extern int fireplaceId;                         ///< Fireplace pick object ID
    // pick object states
    extern bool swordClicked;
    extern bool towerClicked;
    extern bool fireplaceClicked;
}