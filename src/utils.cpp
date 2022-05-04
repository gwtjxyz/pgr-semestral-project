#include "utils.h"

#include <fstream>
#include <iostream>
#include <stb_image.h>

#include "windowing.h"

// helper function for loading a shader file
// TODO maybe prepend 'shaders/' folder to each function usage
std::string loadShaderFromFile(const char * filePath) {
    std::fstream fileIn(filePath, std::ios::in | std::ios::binary);
    if (fileIn) {
        std::string contents;
        fileIn.seekg(0, std::ios::end);
        contents.resize(fileIn.tellg());
        fileIn.seekg(0, std::ios::beg);
        // TODO maybe add some error checking here?
        fileIn.read(&contents[0], contents.size());
        fileIn.close();
        return contents;
    }
    std::cerr << "Couldn't open file " << filePath << '\n';
    return {};
}

// helper function for quick shader compilation
// returns 0 on failure, valid shader id otherwise
// TODO check for GL_INVALID_ENUM?
GLuint compileShader(GLenum shaderType, const char * filePath) {
    GLuint shaderId = 0;
    std::string shader = loadShaderFromFile(filePath);
    if (shader.empty()) {
        std::cerr << "Couldn't compile shader!\n";
        return shaderId;
    }

    const char * shaderSrc = shader.c_str();
    shaderId = glCreateShader(shaderType);
    if (!shaderId) {
        std::cerr << "ERROR creating shader object!\n";
        return shaderId;
    }
    glShaderSource(shaderId, 1, &shaderSrc, nullptr);
    glCompileShader(shaderId);

    // check for errors
    int success;
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        std::cerr << "ERROR compiling shader with path " << shader << ", log:\n" << infoLog << '\n';
    }
    return shaderId;
}

// helper function for linking compiled shader
// assumes the shader has been compiled prior to calling this function
// and that the shader program ID has been initialized
GLuint linkShader(GLuint shaderId) {
    if (!program.activeId) {
        std::cerr << "ERROR attempting to link shader ID " << shaderId << " before creating OpenGL program.\n" <<
        "Make sure you've initialized the program with a glCreateProgram() call before using this function.\n";
    }
    if (!shaderId) {
        std::cerr << "ERROR cannot link shader; Invalid ID of 0.\n" <<
        "Have you compiled the shader before calling this function?\n";
    }

    glAttachShader(program.activeId, shaderId);
    glLinkProgram(program.activeId);

    int success;
    char infoLog[512];
    glGetProgramiv(program.activeId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program.activeId, 512, nullptr, infoLog);
        std::cerr << "ERROR linking shader program! Log:\n" << infoLog << '\n';
    }

    glDeleteShader(shaderId);
    return shaderId;
}

// combines compileShader() and linkShader() together
GLuint createShader(GLenum shaderType, const char * filePath) {
    return linkShader(compileShader(shaderType, filePath));
}

GLuint loadTexture2D(const char * path) {
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char * data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Successfully loaded texture with path '" << path << "'\n";
    } else {
        std::cerr << "Failed to load texture with path '" << path << "'\n";
    }
    stbi_image_free(data);

    return texture;
}

GLuint loadTexture2D(const char * path, GLenum format) {
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char * data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Successfully loaded texture with path '" << path << "'\n";
    } else {
        std::cerr << "Failed to load texture with path '" << path << "'\n";
    }
    stbi_image_free(data);

    return texture;
}