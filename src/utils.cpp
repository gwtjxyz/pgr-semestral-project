#include "utils.h"

#include <fstream>
#include <iostream>
#include <stb_image.h>

#include "windowing.h"
#include "glm/gtc/type_ptr.hpp"

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
        std::cerr << "ERROR compiling shader with path " << filePath << ", log:\n" << infoLog << '\n';
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

void setUniform3f(const char * property, const glm::vec3 & vec) {
    glUniform3f(glGetUniformLocation(program.activeId, property), vec.x, vec.y, vec.z);
}

void setUniform3f(const char * property, const float & x, const float & y, const float & z) {
    glUniform3f(glGetUniformLocation(program.activeId, property), x, y, z);
}

void setUniform2f(const char * property, const glm::vec2 & vec) {
    glUniform2f(glGetUniformLocation(program.activeId, property), vec.x, vec.y);
}

void setUniform2f(const char * property, const float & x, const float & y) {
    glUniform2f(glGetUniformLocation(program.activeId, property), x, y);
}

void setUniform1i(const char * property, const int & in) {
    glUniform1i(glGetUniformLocation(program.activeId, property), in);
}

void setUniform1f(const char * property, const float & f) {
    glUniform1f(glGetUniformLocation(program.activeId, property), f);
}

void setUniformMat4(const char * property, const glm::mat4 & mat) {
    glUniformMatrix4fv(glGetUniformLocation(program.activeId, property), 1, GL_FALSE, glm::value_ptr(mat));
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
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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

GLuint loadTextureFromFile(const char * path, const std::string & directory, bool gamma) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    GLuint textureId = loadTexture2D(filename.c_str());
    return textureId;
}

GLuint loadCubemap(std::vector<std::string> faces) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    int width, height, nrChannels;
    for (int i = 0; i != faces.size(); ++i) {
        unsigned char * data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cerr << "Cubemap texture failed to load at path '" << faces[i] << "'\n";
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureId;
}

// TODO stop being lazy and fill this up
void loadMaterials() {
    Material emerald {
            {0.0215, 0.1745, 0.0215},
            {0.07568, 0.61424, 0.07568},
            {0.633, 0.727811, 0.633},
            0.6
    };
//    gl::materials.emplace(MaterialType::EMERALD, emerald);
}