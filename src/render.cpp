#include "render.h"

#include <string>

#include "utils.h"
#include "windowing.h"

glm::mat4 Render::projection() {
    return glm::perspective(glm::radians(Config::FOV),
                            (float) Config::WINDOW_WIDTH / (float) Config::WINDOW_HEIGHT,
                            Config::ZNEAR,
                            Config::ZFAR);
}

void loadMainShaders() {
    program.activeId = gl::programId;
//    setActiveProgram(gl::programId);
    createShader(GL_VERTEX_SHADER, R"(../shaders/vertexShaderSource.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/fragmentShaderSource.frag)");
    glUseProgram(program.activeId);
}

void loadLightSourceShaders() {
//    setActiveProgram(gl::lightingId);
    program.activeId = gl::lightingId;
    createShader(GL_VERTEX_SHADER, R"(../shaders/vertexShaderLightSrc.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/fragmentShaderLightSrc.frag)");
    glUseProgram(program.activeId);
}

void loadSkyboxShaders() {
    program.activeId = gl::skyboxId;
    createShader(GL_VERTEX_SHADER, R"(../shaders/skybox.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/skybox.frag)");
    glUseProgram(program.activeId);
}

void renderDirectionalLight(const char * varName,
                            glm::vec3 direction,
                            glm::vec3 ambient,
                            glm::vec3 diffuse,
                            glm::vec3 specular) {
    std::string destination = varName;
    setUniform3f((destination + ".direction").c_str(), direction);
    setUniform3f((destination + ".ambient").c_str(), ambient);
    setUniform3f((destination + ".diffuse").c_str(), diffuse);
    setUniform3f((destination + ".specular").c_str(), specular);
}

void renderPointLight(const char * varName,
                      glm::vec3 position,
                      glm::vec3 ambient,
                      glm::vec3 diffuse,
                      glm::vec3 specular,
                      float constant,
                      float linear,
                      float quadratic) {
    std::string destination = varName;
    setUniform3f((destination + ".position").c_str(), position);
    setUniform3f((destination + ".ambient").c_str(), ambient);
    setUniform3f((destination + ".diffuse").c_str(), diffuse);
    setUniform3f((destination + ".specular").c_str(), specular);
    setUniform1f((destination + ".constant").c_str(), constant);
    setUniform1f((destination + ".linear").c_str(), linear);
    setUniform1f((destination + ".quadratic").c_str(), quadratic);
}

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
                     float outerCutoff) {
    std::string destination = varName;
    setUniform3f((destination + ".position").c_str(), position);
    setUniform3f((destination + ".direction").c_str(), direction);
    setUniform3f((destination + ".ambient").c_str(), ambient);
    setUniform3f((destination + ".diffuse").c_str(), diffuse);
    setUniform3f((destination + ".specular").c_str(), specular);
    setUniform1f((destination + ".constant").c_str(), constant);
    setUniform1f((destination + ".linear").c_str(), linear);
    setUniform1f((destination + ".quadratic").c_str(), quadratic);
    setUniform1f((destination + ".cutoff").c_str(), cutoff);
    setUniform1f((destination + ".outerCutoff").c_str(), outerCutoff);

}

void drawTerrain(Terrain terrain, const glm::mat4 & proj, const glm::mat4 & view) {
    // set up textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, terrain.diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, terrain.specular);

    glBindVertexArray(terrain.VAO);
    // move terrain to be centered around [0,0,0]
    glm::mat4 terrainModel = glm::mat4(1.0f);
    terrainModel = glm::translate(terrainModel,
                                  {-((float) terrain.size / 4),
                                   Config::TERRAIN_Y,
                                   -((float) terrain.size / 4)});

    glm::mat4 PVM = proj * view * terrainModel;

    setUniformMat4("PVM", PVM);
    setUniformMat4("model", terrainModel);

    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, terrain.nrTriangles * 3, GL_UNSIGNED_INT, 0);
}

void drawSkybox(Skybox & skybox) {
    glDepthMask(GL_FALSE);
    setActiveProgram(gl::skyboxId);
    glm::mat4 proj = Render::projection();
    glm::mat4 view = glm::mat4(glm::mat3(program.activeCamera.getViewMatrix()));
    setUniformMat4("projection", proj);
    setUniformMat4("view", view);
    setUniform1i("clouds", skybox.cloudsTexture);
//    setUniform2f("CloudCoords", 0.0f, 1.0f);
    glBindVertexArray(skybox.VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.texture);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D,  skybox.cloudsTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void drawBackpack(Model & backpack) {

}

void drawTower(Model & tower) {

    glm::mat4 proj, view, model, PVM;
    proj = Render::projection();
    view = program.activeCamera.getViewMatrix();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-15.0f, 2.0f, -15.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-110.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    PVM = proj * view * model;
    setUniform1f("materials[0].shininess", 4.0f);
    setUniformMat4("model", model);
    setUniformMat4("PVM", PVM);
    tower.draw(gl::programId);
}