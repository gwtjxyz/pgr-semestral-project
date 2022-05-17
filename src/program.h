#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       program.h
 * \author     Yury Udavichenka
 * \date       12/05/2022
 * \brief      Program header
 *
 *  Header containing the program struct that holds immediate application data
 *  as well as several helper functions
 *
*/
//----------------------------------------------------------------------------------------

#include "camera.h"
/// struct containing global runtime variable info
struct Program {
    Camera * activeCamera;          ///< Currently active camera
    GLuint activeId = 0;            ///< Currently active shader program ID
    int enableWarp = 0;             ///< Flag for enabling warping inside main shader
    int enableSpotlight = 0;        ///< Flag for enabling flashlight spotlight
    Camera mainCamera;              ///< User-controlled camera
    Camera staticCamera1;           ///< Static angle #1
    Camera staticCamera2;           ///< Static angle #2
    Camera dynamicCamera;           ///< Dynamic angle without user control
};
/// struct containing global runtime variable info
extern Program program;

void setActiveProgram(GLuint program);

/// function to update the time delta
void updateTime();