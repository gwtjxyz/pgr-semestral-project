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
uint32_t createShader(GLenum shaderType, const char * filePath) {
    uint32_t shaderId = 0;
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