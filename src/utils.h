#pragma once

#include <string>
#include <glad/glad.h>

#include "config.h"

std::string loadShaderFromFile(const char * filePath);

GLuint compileShader(GLenum shaderType, const char * filePath);

GLuint linkShader(GLuint shaderId);

GLuint createShader(GLenum shaderType, const char * filePath);

// Loads and binds texture from specified file.
// Uses GL_REPEAT, RGB and linear mipmapping by default.
// TODO make overloaded versions of this function to change default params
GLuint loadTexture2D(const char * path);

// Loads and binds texture from specified file.
// The texture format is set manually by the user.
// Uses GL_REPEAT, linear mipmapping by default.
GLuint loadTexture2D(const char * path, GLenum format);