#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "config.h"

std::string loadShaderFromFile(const char * filePath);

GLuint compileShader(GLenum shaderType, const char * filePath);

GLuint linkShader(GLuint shaderId);

GLuint createShader(GLenum shaderType, const char * filePath);

void setUniform3f(const char * property, const glm::vec3 & in);

void setUniform3f(const char * property, const float & x, const float & y, const float & z);

void setUniform2f(const char * property, const glm::vec2 & in);

void setUniform2f(const char * property, const float & x, const float & y);

void setUniform1i(const char * property, const int & in);

void setUniform1f(const char * property, const float & f);

void setUniformMat4(const char * property, const glm::mat4 & mat);

// Loads and binds texture from specified file.
// Uses GL_REPEAT, RGB and linear mipmapping by default.
// TODO make overloaded versions of this function to change default params
GLuint loadTexture2D(const char * path);

// Loads and binds texture from specified file.
// The texture format is set manually by the user.
// Uses GL_REPEAT, linear mipmapping by default.
GLuint loadTexture2D(const char * path, GLenum format);

GLuint loadTextureFromFile(const char * path, const std::string & directory, bool gamma = false);

GLuint loadCubemap(std::vector<std::string> faces);

void loadMaterials();

void setupFramebuffer();