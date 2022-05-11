#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "config.h"
#include "windowing.h"
#include "utils.h"
#include "render.h"
#include "world/terrain.h"
#include "objects/objects.h"

// loads shaders from files and links them into the program

void drawTenCubes(const glm::mat4 & proj, const glm::mat4 & view, const glm::vec3 * cubePositions) {

    for (int i = 0; i != 10; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * (float) i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        glm::mat4 PVM = proj * view * model;
        setUniformMat4("PVM", PVM);
        setUniformMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void drawFog() {
    glm::vec3 fogCenter = program.activeCamera.mPosition;
    setUniform3f("fogCenter", fogCenter);
}

int main() {
    if (!init()) {
        glfwTerminate();
        return -1;
    }

    // vertices and their respective buffers
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // ==============================================================
    // main scene
    loadMainShaders();

    setActiveProgram(gl::programId);
    setUniform1i("materials[0].diffuse", 0);
    setUniform1i("materials[0].specular", 1);
//    setUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
    setUniform1f("materials[0].shininess", 32.0f);

    GLuint cubeVAO, VBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    // normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    stbi_set_flip_vertically_on_load(true);
    GLuint diffuseMap = loadTexture2D(R"(../resources/container2.png)");
    GLuint specularMap = loadTexture2D(R"(../resources/container2_specular.png)");

    Model backpackModel(R"(../resources/models/backpack/backpack.obj)");
    Model towerModel(R"(../resources/models/tower/scene.gltf)");
    Model swordModel(R"(../resources/models/sword/Sting.obj)");

    Terrain terrain = createTerrain(Config::TERRAIN_SIZE, Config::TERRAIN_TEXTURE_STEP);

    // ================================================================
    // skybox
    stbi_set_flip_vertically_on_load(false);
    Skybox skybox = loadSkybox();
    stbi_set_flip_vertically_on_load(true);

    // ================================================================
    // lighting - light source creation
    loadLightSourceShaders();

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO data already has the needed data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    // ==============================================================
    // logo
    loadLogoShaders();
    Image logo = loadLogo();


    // main loop
    while (!glfwWindowShouldClose(gl::mainWindow)) {

        // per-frame time logic
        float currentFrame = (float) glfwGetTime();
        gl::deltaTime = currentFrame - gl::lastFrame;
        gl::lastFrame = currentFrame;
        lightPos.y = sin(glfwGetTime());

        // input
        processInput(gl::mainWindow);

        if (Config::ENABLE_DEBUG)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // render
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawSkybox(skybox);

        // set up PVM
        glm::mat4 proj = Render::projection();

        glm::mat4 view = program.activeCamera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 PVM;

//=============================================================================
        // render light
        setActiveProgram(gl::lightingId);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        PVM = proj * view * model;

        setUniformMat4("PVM", PVM);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

//=============================================================================
        // render cubes
        setActiveProgram(gl::programId);
        setUniform1i("warp.flagWarp", program.enableWarp);
        setUniform1i("flagSpotlight", program.enableSpotlight);
        setUniform3f("viewPos", program.activeCamera.mPosition);
        setUniform1f("warp.warpBy", (float) sin(glfwGetTime()));

        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
//        lightColor.x = sin(glfwGetTime() * 2.0f);
//        lightColor.y = sin(glfwGetTime() * 0.7f);
//        lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);

        drawFog();

        // directional light
        renderDirectionalLight("dirLight",
                               {-0.2f, -1.0f, -0.3f},
                               {0.06f, 0.06f, 0.06f},
                               {0.4f, 0.4f, 0.4f},
                               {0.5f, 0.5f, 0.5f});

        // point lights
        for (int i = 0; i != 3; ++i) {
            std::string destination = "pointLights[";
            destination.append(std::to_string(i)).append("]");
            renderPointLight(destination.c_str(),
                             pointLightPositions[i],
                             {0.05f, 0.05f, 0.05f},
                             {0.8f, 0.8f, 0.8f},
                             {1.0f, 1.0f, 1.0f},
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

        // spot light
        renderSpotlight("spotlight",
                        program.activeCamera.mPosition,
                        program.activeCamera.mFront,
                        {0.0f, 0.0f, 0.0f},
                        {1.0f, 1.0f, 1.0f},
                        {1.0f, 1.0f, 1.0f},
                        1.0f,
                        0.09f,
                        0.032f,
                        glm::cos(glm::radians(12.5f)),
                        glm::cos(glm::radians(15.0f)));

        // textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(cubeVAO);
        drawTenCubes(proj, view, cubePositions);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        PVM = proj * view * model;
        setUniformMat4("model", model);
        setUniformMat4("PVM", PVM);
        backpackModel.draw(gl::programId);

        drawTower(towerModel);

        drawSword(swordModel);

        // terrain (please work)
        drawTerrain(terrain, proj, view);

        // logo

        drawLogo(logo, view, proj);

        // check and call events and swap the buffers
        glfwSwapBuffers(gl::mainWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    deleteTerrain(terrain);

    glfwTerminate();
    return 0;
}
