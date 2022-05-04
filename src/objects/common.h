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

// TODO this is stupid, change it
struct Object {
    unsigned int id = 0;
    std::string name;
    float * vertices = nullptr;
    float * indices = nullptr;
    float * colors = nullptr;
    std::string textureName;
    float * textureIndices = nullptr;
    bool placed = false;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
};

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};