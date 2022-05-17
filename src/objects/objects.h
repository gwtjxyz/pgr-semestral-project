#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       objects.h
 * \author     Yury Udavichenka
 * \date       01/05/2022
 * \brief      object loading
 *
 * some structs and functions for object loading.
 *
*/
//----------------------------------------------------------------------------------------
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "model.h"

struct Skybox {
    GLuint VAO;
    GLuint VBO;
    GLuint texture;
};

struct Image {
    GLuint VAO;
    GLuint VBO;
    GLuint texture;
    glm::vec3 position;
    float time;
};

Object loadCubes();
/// loads light source cube with light sources shaders
/// @param[in] otherCube pre-generated cube object (we use its vertex buffers)
Object loadLightSourceCube(Object & otherCube);

Skybox loadSkybox();

Model loadBackpack();

Image loadLogo();

Image loadFire();