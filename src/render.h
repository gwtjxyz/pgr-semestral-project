#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       render.h
 * \author     Yury Udavichenka
 * \date       18/04/2022
 * \brief      render functions header
 *
 *  Contains several OpenGL rendering-related classes and functions
 *
*/
//----------------------------------------------------------------------------------------

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "config.h"
#include "world/terrain.h"
#include "objects/objects.h"

namespace Render {
    // model - transforms object space to world space


    // view - transforms world space to camera space


    // projection - transforms camera space to clip space
    glm::mat4 projection();
}

void loadMainShaders();

void loadLightSourceShaders();

void loadSkyboxShaders();

void renderDirectionalLight(const char * varName,
                            glm::vec3 direction,
                            glm::vec3 ambient,
                            glm::vec3 diffuse,
                            glm::vec3 specular);

void renderPointLight(const char * varName,
                      glm::vec3 position,
                      glm::vec3 ambient,
                      glm::vec3 diffuse,
                      glm::vec3 specular,
                      float constant,
                      float linear,
                      float quadratic);

void renderSpotlight(const char * varName,
                     glm::vec3 position,
                     glm::vec3 direction,
                     glm::vec3 ambient,
                     glm::vec3 diffuse,
                     glm::vec3 specular,
                     float constant,
                     float linear,
                     float quadratic,
                     float cutoff,
                     float outerCutoff);

void loadLogoShaders();

void drawTerrain(Terrain terrain, const glm::mat4 & proj, const glm::mat4 & view);

void drawSkybox(Skybox & skybox);

void drawBackpack(Model & backpack);

void drawTower(Model & tower);

void drawLogo(Image & logo, const glm::mat4 & view, const glm::mat4 & proj);