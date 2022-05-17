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