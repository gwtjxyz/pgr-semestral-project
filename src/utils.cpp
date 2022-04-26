#include "utils.h"

#include <fstream>
#include <iostream>

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
    if (!gl::programId) {
        std::cerr << "ERROR attempting to link shader ID " << shaderId << " before creating OpenGL program.\n" <<
        "Make sure you've initialized the program with a glCreateProgram() call before using this function.\n";
    }
    if (!shaderId) {
        std::cerr << "ERROR cannot link shader; Invalid ID of 0.\n" <<
        "Have you compiled the shader before calling this function?\n";
    }

    glAttachShader(gl::programId, shaderId);
    glLinkProgram(gl::programId);

    int success;
    char infoLog[512];
    glGetProgramiv(gl::programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(gl::programId, 512, nullptr, infoLog);
        std::cerr << "ERROR linking shader program! Log:\n" << infoLog << '\n';
    }

    glDeleteShader(shaderId);
    return shaderId;
}

// combines compileShader() and linkShader() together
GLuint createShader(GLenum shaderType, const char * filePath) {
    return linkShader(compileShader(shaderType, filePath));
}