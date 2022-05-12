#pragma once

#include "camera.h"

struct Program {
    Camera activeCamera;
    GLuint activeId = 0;
    int enableWarp = 0;
    int enableSpotlight = 0;
    bool isClickHeldDown = false;
};

extern Program program;

void setActiveProgram(GLuint program);