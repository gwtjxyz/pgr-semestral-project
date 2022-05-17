#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       common.h
 * \author     Yury Udavichenka
 * \date       03/05/2022
 * \brief      common object-related things
 *
 *  Some unused things as well as some very important data structures for storing
 *  object, vertex or model data.
 *
*/
//----------------------------------------------------------------------------------------
#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// we check what's loaded into the mesh using bit operators
#define MESH_TYPE_VERTEX 1          ///< unused
#define MESH_TYPE_NORMAL 2          ///< unused
#define MESH_TYPE_TEXTURE 4         ///< unused

/// unused
struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};
/// Vertex utility struct
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};
/// Texture utility struct
struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};
/// Generic object struct - for things simpler than something loaded from external files
struct Object {
    GLuint VAO;             ///< Vertex array object ID
    GLuint VBO;             ///< Vertex buffer object ID
    GLuint specular;        ///< Specular texture ID
    GLuint diffuse;         ///< Diffuse texture ID
};

/// unused
enum class LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

/// unused
struct Light {
    LightType type;
    glm::vec3 position;         // not necessary for directional
    glm::vec3 direction;        // not necessary for point light

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;             // not necessary for directional
    float linear;               // not necessary for directional
    float quadratic;            // not necessary for directional

    float cutoff;               // specific for spot
    float outerCutoff;          // specific for spot
};