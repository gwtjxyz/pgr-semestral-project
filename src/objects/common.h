#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       common.h
 * \author     Yury Udavichenka
 * \date       03/05/2022
 * \brief      A brief file description
 *
 *  A more detailed file description.
 *
*/
//----------------------------------------------------------------------------------------
#include <string>
#include <glm/glm.hpp>

// we check what's loaded into the mesh using bit operators
#define MESH_TYPE_VERTEX 1
#define MESH_TYPE_NORMAL 2
#define MESH_TYPE_TEXTURE 4

struct Mesh {
    unsigned int type;
    float * data = nullptr;
    GLuint program;
};

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

enum class LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

// Generic light container, not every field has to be
// filled out - depends on the light
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