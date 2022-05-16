#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "config.h"
#include "program.h"
#include "windowing.h"
#include "utils.h"
#include "render.h"
#include "world/terrain.h"
#include "objects/objects.h"

// loads shaders from files and links them into the program

int main() {
    if (!init()) {
        glfwTerminate();
        return -1;
    }

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // ==============================================================
    // object selection
    loadPickObjectShaders();
    // main scene
    loadMainShaders();

    setActiveProgram(gl::programId);
//    setUniform1i("materials[0].diffuse", 0);
//    setUniform1i("materials[0].specular", 1);
    setUniform1f("materials[0].shininess", 32.0f);

    Object cubes = loadCubes();

    Model backpackModel(R"(../resources/models/backpack/backpack.obj)");

    Model treeModel(R"(../resources/models/dead_tree/tree_.fbx)");

    Model towerModel(R"(../resources/models/tower/scene.gltf)");
    // do this because this model's textures are broken otherwise
    GLuint swordDiff = loadTexture2D(R"(../resources/models/sword/shortsword_Shortsword_Diffuse.png)");
    GLuint swordSpec = loadTexture2D(R"(../resources/models/sword/shortsword_Shortsword_Reflection.png)");
    Model swordModel(R"(../resources/models/sword/shortsword.obj)");
    // same here
    GLuint fireplaceDiff = loadTexture2D(R"(../resources/models/fireplace/ohniste4UVcomplet1.png)");
    GLuint fireplaceSpec = loadTexture2D(R"(../resources/models/fireplace/ohniste4UVcomplet1spec.png)");
    Model fireplaceModel(R"(../resources/models/fireplace/fireplace.3ds)");

    Model gullModel(R"(../resources/models/bird/bird static.fbx)");

    Terrain terrain = createTerrain(Config::TERRAIN_SIZE, Config::TERRAIN_TEXTURE_STEP);

    // ==============================================================
    // skybox
    Skybox skybox = loadSkybox();
    // ==============================================================
    // lighting - light source creation
    loadLightSourceShaders();
    Object lightSourceCube = loadLightSourceCube(cubes);
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    // ==============================================================
    // logo
    loadLogoShaders();
    Image logo = loadLogo();
    // ===============================================================
    // fire texture animation
    loadFireShaders();
    Image fire = loadFire();
    // ===============================================================
    // pick framebuffer
    setupFramebuffer();
    // ===============================================================
    // main loop
    while (!glfwWindowShouldClose(gl::mainWindow)) {

        // per-frame time logic
        updateTime();
        lightPos.y = sin(glfwGetTime());
//        lightPos.x = cos(glfwGetTime());

        // input
        processInput(gl::mainWindow);

        // wireframes
        if (Config::ENABLE_DEBUG)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // clear
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        clearPickFramebuffer();

        // set up PVM
        glm::mat4 proj = Render::projection();
        glm::mat4 view = program.activeCamera->getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 PVM;
//=============================================================================
        // render skybox
        drawSkybox(skybox);
//=============================================================================
        // render bouncing cube
        drawLightCube(lightPos, proj, view, lightSourceCube.VAO);
//=============================================================================
        // set up main shader uniforms
        setActiveProgram(gl::programId);
        setUniform1i("warp.flagWarp", program.enableWarp);
        setUniform1i("flagSpotlight", program.enableSpotlight);
        setUniform3f("viewPos", program.activeCamera->mPosition);
        setUniform1f("warp.warpBy", (float) sin(glfwGetTime()));
        // render objects

        drawFog();
        drawLights(lightPos, pointLightPositions);
        drawTenCubes(proj, view, cubes);
        drawBackpack(backpackModel);
        drawTrees(treeModel);
        drawTower(towerModel);
        drawSword(swordModel, swordDiff, swordSpec, gl::lastFrame);
        drawFireplace(fireplaceModel, fireplaceDiff, fireplaceSpec);
        drawSeagull(gullModel);

        // terrain
        drawTerrain(terrain, proj, view);

        // images
        drawLogo(logo, view, proj);
        drawFire(fire, view, proj);

        // check and call events and swap the buffers
        glfwSwapBuffers(gl::mainWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubes.VAO);
    glDeleteBuffers(1, &cubes.VBO);
    deleteTerrain(terrain);

    glfwTerminate();
    return 0;
}
