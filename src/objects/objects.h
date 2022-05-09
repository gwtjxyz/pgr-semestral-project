#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Skybox {
    GLuint VAO;
    GLuint VBO;
    GLuint texture;
};

Skybox loadSkybox();