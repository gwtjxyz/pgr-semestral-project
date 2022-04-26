#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       render.h
 * \author     Yury Udavichenka
 * \date       18/04/2022
 * \brief      render functions header
 *
 *  Contains several OpenGL rendering-related functions
 *
*/
//----------------------------------------------------------------------------------------

#include <glad/glad.h>

// Loads and binds texture from specified file.
// Uses GL_REPEAT, RGB and linear mipmapping by default.
// TODO make overloaded versions of this function to change default params
GLuint loadTexture2D(const char * path);

// Loads and binds texture from specified file.
// The texture format is set manually by the user.
// Uses GL_REPEAT, linear mipmapping by default.
GLuint loadTexture2D(const char * path, GLenum format);