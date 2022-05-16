//----------------------------------------------------------------------------------------
/**
 * \file       program.cpp
 * \author     Yury Udavichenka
 * \date       12/05/2022
 * \brief      A brief file description
 *
 *  A more detailed file description.
 *
*/
//----------------------------------------------------------------------------------------
#include "program.h"

Program program;

void setActiveProgram(GLuint id) {
    program.activeId = id;
    glUseProgram(program.activeId);
}

void updateTime() {
    float currentFrame = (float) glfwGetTime();
    gl::deltaTime = currentFrame - gl::lastFrame;
    gl::lastFrame = currentFrame;
}