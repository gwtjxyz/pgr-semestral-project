#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       render.h
 * \author     Yury Udavichenka
 * \date       18/04/2022
 * \brief      render functions header
 *
 *  Contains several OpenGL rendering-related classes and functions
 *
*/
//----------------------------------------------------------------------------------------

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "config.h"

namespace Render {
    // model - transforms object space to world space


    // view - transforms world space to camera space


    // projection - transforms camera space to clip space
    glm::mat4 projection();
}