//----------------------------------------------------------------------------------------
/**
 * \file       config.cpp
 * \author     Yury Udavichenka
 * \date       16/04/2022
 * \brief      Description of bookmark_dialog class (A brief file description).
 *
 *  A more elaborated file description.
 *
*/
//----------------------------------------------------------------------------------------
#include "config.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Config {
    int WINDOW_WIDTH = 800;
    int WINDOW_HEIGHT = 600;
    bool ENABLE_DEBUG = false;
    float FOV = 59.0f;          // vertical FOV - equal to 90° horizontal FOV
    float ZNEAR = 0.1f;
    float ZFAR = 100.0f;


}

namespace gl {
    GLuint programId = 0;
    GLuint lightingId = 0;
    GLFWwindow * mainWindow = nullptr;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    bool firstMouse = true;
    float lastX = Config::WINDOW_WIDTH / 2.0f;
    float lastY = Config::WINDOW_HEIGHT / 2.0f;
}