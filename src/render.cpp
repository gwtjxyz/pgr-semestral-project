#include "render.h"

#include <string>

#include "program.h"
#include "utils.h"

glm::mat4 Render::projection() {
    return glm::perspective(glm::radians(Config::FOV),
                            (float) Config::WINDOW_WIDTH / (float) Config::WINDOW_HEIGHT,
                            Config::ZNEAR,
                            Config::ZFAR);
}

void loadMainShaders() {
    program.activeId = gl::programId;
//    setActiveProgram(gl::programId);
    createShader(GL_VERTEX_SHADER, R"(../shaders/mainShader.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/mainShader.frag)");
    glUseProgram(program.activeId);
}

void loadLightSourceShaders() {
//    setActiveProgram(gl::lightingId);
    program.activeId = gl::lightingId;
    createShader(GL_VERTEX_SHADER, R"(../shaders/lightSrc.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/lightSrc.frag)");
    glUseProgram(program.activeId);
}

void loadSkyboxShaders() {
    program.activeId = gl::skyboxId;
    createShader(GL_VERTEX_SHADER, R"(../shaders/skybox.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/skybox.frag)");
    glUseProgram(program.activeId);
}

void loadLogoShaders() {
    program.activeId = gl::logoId;
    createShader(GL_VERTEX_SHADER, R"(../shaders/logoShader.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/logoShader.frag)");
    glUseProgram(program.activeId);
}

void loadPickObjectShaders() {
    program.activeId = gl::pickObjectId;
    createShader(GL_VERTEX_SHADER, R"(../shaders/pickShader.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/pickShader.frag)");
    glUseProgram(program.activeId);
}

void loadFireShaders() {
    program.activeId = gl::fireId;
    createShader(GL_VERTEX_SHADER, R"(../shaders/fire.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/fire.frag)");
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

void clearPickFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, gl::pickFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void drawPickBuffer(const glm::mat4 & proj, const glm::mat4 & view, const glm::mat4 & model, const Model & object, const GLuint id) {
    glBindFramebuffer(GL_FRAMEBUFFER, gl::pickFBO);

    GLuint oldProgram = program.activeId;
    setActiveProgram(gl::pickObjectId);
    setUniformMat4("proj", proj);
    setUniformMat4("view", view);
    for (auto & mesh : object.mMeshes) {
        glBindVertexArray(mesh.mVAOpick);
        setUniform1f("id", (float) id / 255);
        setUniformMat4("model", model);
        glDrawElements(GL_TRIANGLES, mesh.mIndices.size(), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    setActiveProgram(oldProgram);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void drawLightCube(const glm::vec3 & lightPos,
                   const glm::mat4 & proj,
                   const glm::mat4 & view,
                   GLuint VAO) {
    setActiveProgram(gl::lightingId);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    glm::mat4 PVM = proj * view * model;

    setUniformMat4("PVM", PVM);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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
//    glActiveTexture(GL_TEXTURE6);
//    glBindTexture(GL_TEXTURE_2D,  skybox.cloudsTexture);
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
//    setUniform1f("materials[0].shininess", 4.0f);
    setUniformMat4("model", model);
    setUniformMat4("PVM", PVM);
    tower.draw(gl::programId);

    if (gl::towerClicked) {
        setUniform1i("flagTowerSpotlight", 1);
    } else {
        setUniform1i("flagTowerSpotlight", 0);
    }

    drawPickBuffer(proj, view, model, tower, gl::towerId);
}

void drawLogo(Image & logo, const glm::mat4 & view, const glm::mat4 & proj) {
    if (!gl::logoEnabled) {
        return;
    }

    setActiveProgram(gl::logoId);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(2.0f));
    model = glm::translate(model, logo.position);

    // we want to use orthogonal matrices since it's an overlay
    glm::mat4 orthoProj = glm::ortho(
            -1.0f,1.0f,
            -1.0f, 1.0f,
            -10.0f, 10.0f
    );
    glm::mat4 orthoView = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glm::mat4 PVM = orthoProj * orthoView * model;
    setUniformMat4("PVM", PVM);
    setUniform1i("texSampler", 0);
    setUniform1f("time", gl::logoTime);
    glBindVertexArray(logo.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, logo.texture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    gl::logoTime += gl::deltaTime;

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void drawFire(Image & fire, const glm::mat4 & view, const glm::mat4 & proj) {
    if (gl::fireplaceClicked)
        return;

    setActiveProgram(gl::fireId);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-12.0f, -5.3f, -10.0f));
    model = glm::rotate(model,
                        glm::radians(-180.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 PVM = proj * view * model;
    setUniformMat4("PVM", PVM);
    setUniform1i("texSampler", 0);
    setUniform1f("time", gl::fireTime);
    glBindVertexArray(fire.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fire.texture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    model = glm::rotate(model,
                        glm::radians(90.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    PVM = proj * view * model;
    setUniformMat4("PVM", PVM);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    gl::fireTime += gl::deltaTime;
    // gl::fireTime += 0.1f;

    glDisable(GL_BLEND);
}

void drawSword(Model & sword, GLuint diff, GLuint spec, const float & time) {
    glm::mat4 proj, view, model, PVM;
    proj = Render::projection();
    view = program.activeCamera.getViewMatrix();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.0f, -5.3f, 10.0f));
    if (gl::swordClicked) {
        model = glm::rotate(model,
                            (float) glm::radians(sin(time) * 360.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f));
    }
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    PVM = proj * view * model;
//    setUniform1f("materials[0].shininess", 4.0f);
    setUniformMat4("model", model);
    setUniformMat4("PVM", PVM);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diff);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, spec);
    sword.draw(gl::programId);

    // object picking
    drawPickBuffer(proj, view, model, sword, gl::swordId);
}

void drawFireplace(Model & fireplace, GLuint diff, GLuint spec) {
    glm::mat4 proj, view, model, PVM;
    proj = Render::projection();
    view = program.activeCamera.getViewMatrix();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-12.0f, -5.3f, -10.0f));
    model = glm::rotate(model,
                        glm::radians(-90.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    PVM = proj * view * model;
//    setUniform1f("materials[0].shininess", 4.0f);
    setUniform1f("materials[0].shininess", 1.0f);
    setUniformMat4("model", model);
    setUniformMat4("PVM", PVM);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diff);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, spec);
    fireplace.draw(gl::programId);

    // object picking
    drawPickBuffer(proj, view, model, fireplace, gl::fireplaceId);
}