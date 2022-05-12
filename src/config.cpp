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



namespace Config {
    int WINDOW_WIDTH = 800;
    int WINDOW_HEIGHT = 600;
    bool ENABLE_DEBUG = false;
    bool ENABLE_FULLSCREEN = false;
    float FOV = 59.0f;          // vertical FOV - equal to 90° horizontal FOV
    float ZNEAR = 0.1f;
    float ZFAR = 100.0f;


}

namespace gl {
    GLuint pickFBO = 0;
    GLuint programId = 0;
    GLuint lightingId = 0;
    GLuint skyboxId = 0;
    GLuint logoId = 0;
    GLuint pickObjectId = 0;
    GLFWwindow * mainWindow = nullptr;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    bool firstMouse = true;
    float lastX = Config::WINDOW_WIDTH / 2.0f;
    float lastY = Config::WINDOW_HEIGHT / 2.0f;
    bool logoEnabled = false;
    float logoTime = 0.0f;
    bool enableMovement = true;
    int swordId = 1;
}