#pragma once

#include <string>
#include <glad/glad.h>

std::string loadShaderFromFile(const char * filePath);

uint32_t createShader(GLenum shaderType, const char * filePath);