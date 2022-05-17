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
    createShader(GL_VERTEX_SHADER, R"(../shaders/mainShader.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/mainShader.frag)");
    glUseProgram(program.activeId);
}

void loadLightSourceShaders() {
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

void drawCrates(const glm::mat4 & proj, const glm::mat4 & view, Object & cube) {
    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  -5.0f,  0.0f),
            glm::vec3( 0.0f,  -4.0f, 0.0f),
            glm::vec3(-0.1f, -3.0f, -0.2f),
            glm::vec3(-0.1f, -2.0f, 0.1f),
            glm::vec3( 0.0f, -1.0f, -0.0f),
            glm::vec3(-1.7f,  -5.0f, -7.5f),
            glm::vec3( -3.3f, -5.0f, -2.5f),
            glm::vec3( 1.5f,  -5.0f, -2.5f),
            glm::vec3( 2.5f,  -5.2f, -1.5f),
            glm::vec3(-2.3f,  -5.0f, -3.5f)
    };

    setUniform1f("materials[0].shininess", 32.0f);
    // textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube.diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cube.specular);

    glBindVertexArray(cube.VAO);

    for (int i = 0; i != 10; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * (float) i;
        if (i > 5)
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        glm::mat4 PVM = proj * view * model;
        setUniformMat4("PVM", PVM);
        setUniformMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
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

void drawFog() {
    glm::vec3 fogCenter = program.activeCamera->mPosition;
    setUniform3f("fogCenter", fogCenter);
}

void drawLights(const glm::vec3 & lightPos, const glm::vec3 * pointLightPositions) {
    // directional light
    renderDirectionalLight("dirLight",
                           {-0.2f, -1.0f, -0.3f},
                           {0.3f, 0.3f, 0.3f},
                           {0.4f, 0.4f, 0.4f},
                           {0.5f, 0.5f, 0.5f});

    // point lights
    for (int i = 0; i != 3; ++i) {
        std::string destination = "pointLights[";
        destination.append(std::to_string(i)).append("]");
        renderPointLight(destination.c_str(),
                         pointLightPositions[i],
                         {0.05f, 0.05f, 0.05f},
                         {0.4f, 0.4f, 0.4f},
                         {0.7f, 0.7f, 0.7f},
                         1.0f,
                         0.09f,
                         0.032f);
    }
    std::string destination = "pointLights[3]";
    renderPointLight(destination.c_str(),
                     lightPos,
                     {sin(glfwGetTime() * 0.7f), sin(glfwGetTime()) * 0.05f, sin(glfwGetTime()) * 0.15f},
                     {sin(glfwGetTime()) * 0.6f, sin(glfwGetTime()) * 2.0f, sin(glfwGetTime()) * 1.4f},
                     {1.0f, 1.0f, 1.0f},
                     1.0f,
                     0.14f,
                     0.07f);

    // spotlight
    renderSpotlight("spotlight",
                    program.activeCamera->mPosition,
                    program.activeCamera->mFront,
                    {0.0f, 0.0f, 0.0f},
                    {1.0f, 1.0f, 1.0f},
                    {1.0f, 1.0f, 1.0f},
                    1.0f,
                    0.09f,
                    0.032f,
                    glm::cos(glm::radians(12.5f)),
                    glm::cos(glm::radians(15.0f)));

    // tower light
    renderSpotlight("towerSpotlight",
                    {-15.0f, 30.0f, -15.0f},
                    {0.0f, -90.0f, 0.0f},
                    {0.1f, 0.0f, 0.0f},
                    {1.0f, 0.1f, 0.1f},
                    {1.0f, 0.1f, 0.1f},
                    1.0f,
                    0.09f,
                    0.032f,
                    glm::cos(glm::radians(25.5f)),
                    glm::cos(glm::radians(40.0f)));
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
    glm::mat4 view = glm::mat4(glm::mat3(program.activeCamera->getViewMatrix()));
    setUniformMat4("projection", proj);
    setUniformMat4("view", view);

    glBindVertexArray(skybox.VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.texture);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void drawBackpack(Model & backpack) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = program.activeCamera->getViewMatrix();
    glm::mat4 proj = Render::projection();
    model = glm::translate(model, glm::vec3(-13.5f, -5.0f, -11.5f));
//    model = glm::rotate(model, glm::radians(-170.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
    glm::mat4 PVM = proj * view * model;
    setUniformMat4("model", model);
    setUniformMat4("PVM", PVM);
    backpack.draw(gl::programId);
}

void drawTrees(Model & tree) {
    glm::vec3 treePositions[] = {
            {-5.0f, 4.7f, -5.0f},
            {-5.0f, 4.7f, 7.0f},
            {7.0f, 4.7f, 6.8f},
            {11.0f, 4.7f, -13.0f},
            {9.3f, 4.7f, -8.3f}
    };

    float treeRotations[] = {
            0.0f,
            35.0f,
            13.0f,
            123.0f,
            86.0f
    };

    glm::mat4 proj, view, model, PVM;
    proj = Render::projection();
    view = program.activeCamera->getViewMatrix();
    for (int i = 0; i != 5; ++i) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, treePositions[i]);
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(treeRotations[i]), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        PVM = proj * view * model;
//    setUniform1f("materials[0].shininess", 4.0f);
        setUniformMat4("model", model);
        setUniformMat4("PVM", PVM);
        tree.draw(gl::programId);
    }
}

void drawTower(Model & tower) {
    glm::mat4 proj, view, model, PVM;
    proj = Render::projection();
    view = program.activeCamera->getViewMatrix();
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

    glDisable(GL_BLEND);
}

void drawSword(Model & sword, GLuint diff, GLuint spec) {
    glm::mat4 proj, view, model, PVM;
    proj = Render::projection();
    view = program.activeCamera->getViewMatrix();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.0f, -5.3f, -10.0f));
    if (gl::swordClicked) {
        model = glm::rotate(model,
                            (float) glm::radians(sin(gl::lastFrame) * 360.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f));
    }
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    PVM = proj * view * model;

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
    view = program.activeCamera->getViewMatrix();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-12.0f, -5.3f, -10.0f));
    model = glm::rotate(model,
                        glm::radians(-90.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    PVM = proj * view * model;

    setUniform1f("materials[0].shininess", 16.0f);
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

void drawBird(Model & gull) {
    glm::mat4 proj, view, model, PVM;
    proj = Render::projection();
    view = program.activeCamera->getViewMatrix();
    model = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(-15.0f, 14.0f, -15.0f);
    static float angle = -70.0f;
    angle += 65.0f * gl::deltaTime;
    position.x += sin(glfwGetTime()) * 4.0f;
    position.z += cos(glfwGetTime()) * 4.0f;
    program.dynamicCamera.mPosition = position;
    program.dynamicCamera.mPosition.y += 3.0f;
    program.dynamicCamera.mYaw += 65.0f * gl::deltaTime;
    program.dynamicCamera.updateCameraVectors();
    model = glm::translate(model, position);
    model = glm::rotate(model,
                        glm::radians(angle),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    PVM = proj * view * model;

    setUniformMat4("model", model);
    setUniformMat4("PVM", PVM);
    gull.draw(gl::programId);
}

void drawStele(Model & stele, GLuint diff, GLuint spec) {
    glm::mat4 proj, view, model, PVM;
    proj = Render::projection();
    view = program.activeCamera->getViewMatrix();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-8.0f, -7.0f, 2.0f));
    model = glm::rotate(model,
                        glm::radians(-90.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
    PVM = proj * view * model;

    setUniform1f("materials[0].shininess", 16.0f);
    setUniformMat4("model", model);
    setUniformMat4("PVM", PVM);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diff);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, spec);
    stele.draw(gl::programId);
}

void drawLavaRock(Model & lavaRock, GLuint diff, GLuint spec) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = program.activeCamera->getViewMatrix();
    glm::mat4 proj = Render::projection();
    model = glm::translate(model, glm::vec3(-3.5f, -4.5f, -26.5f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
    glm::mat4 PVM = proj * view * model;
    setUniformMat4("model", model);
    setUniformMat4("PVM", PVM);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diff);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, spec);
    lavaRock.draw(gl::programId);
}

void drawBarrel(Model & barrel, GLuint diff, GLuint spec) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = program.activeCamera->getViewMatrix();
    glm::mat4 proj = Render::projection();
    model = glm::translate(model, glm::vec3(-15.5f, -4.0f, -21.5f));
//    model = glm::rotate(model, glm::radians(-170.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    glm::mat4 PVM = proj * view * model;
    setUniformMat4("model", model);
    setUniformMat4("PVM", PVM);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diff);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, spec);
    barrel.draw(gl::programId);
}