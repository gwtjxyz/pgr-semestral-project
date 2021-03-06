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
    float FOV = 59.0f;
    float ZNEAR = 0.1f;
    float ZFAR = 100.0f;


}

namespace gl {
    nanogui::Screen * screen = nullptr;
    nanogui::FormHelper * gui = nullptr;
    bool guiEnabled = false;
    GLuint pickFBO = 0;
    GLuint programId = 0;
    GLuint lightingId = 0;
    GLuint skyboxId = 0;
    GLuint logoId = 0;
    GLuint pickObjectId = 0;
    GLuint fireId = 0;
    GLFWwindow * mainWindow = nullptr;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    bool firstMouse = true;
    float lastX = Config::WINDOW_WIDTH / 2.0f;
    float lastY = Config::WINDOW_HEIGHT / 2.0f;
    bool logoEnabled = false;
    float logoTime = 0.0f;
    float fireTime = 0.0f;
    // pick object IDs
    int swordId = 1;
    int towerId = 2;
    int fireplaceId = 3;
    // pick object states
    bool swordClicked = false;
    bool towerClicked = false;
    bool fireplaceClicked = false;
}