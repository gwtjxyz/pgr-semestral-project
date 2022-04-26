#pragma once

#include <string>
#include <glad/glad.h>

#include "config.h"

std::string loadShaderFromFile(const char * filePath);

GLuint compileShader(GLenum shaderType, const char * filePath);

GLuint linkShader(GLuint shaderId);

GLuint createShader(GLenum shaderType, const char * filePath);