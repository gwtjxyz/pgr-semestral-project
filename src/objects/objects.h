#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "model.h"

struct Skybox {
    GLuint VAO;
    GLuint VBO;
    GLuint texture;
    GLuint cloudsTexture;   // doesn't work or do anything
};

struct Image {
    GLuint VAO;
    GLuint VBO;
    GLuint texture;
    glm::vec3 position;
    float time;
};

Skybox loadSkybox();

Model loadBackpack();

Image loadLogo();