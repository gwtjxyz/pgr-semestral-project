#include <iostream>
#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "config.h"
#include "windowing.h"
#include "utils.h"
#include "render.h"
#include "world/terrain.h"
//#include "objects/common.h"

// loads shaders from files and links them into the program

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

    // load shaderinos
    loadMainShaders();
//    switch (glGetError()) {
//        case GL_NO_ERROR:
//            break;
//        case GL_INVALID_ENUM:
//            std::cerr << "GL ERROR: INVALID ENUM" << std::endl;
//            break;
//        case GL_INVALID_VALUE:
//            std::cerr << "GL ERROR: INVALID VALUE" << std::endl;
//            break;
//        case GL_INVALID_OPERATION:
//            std::cerr << "GL ERROR: INVALID OPERATION" << std::endl;
//            break;
//        case GL_INVALID_FRAMEBUFFER_OPERATION:
//            std::cerr << "GL ERROR: INVALID FRAMEBUFFER OPERATION" << std::endl;
//            break;
//        case GL_OUT_OF_MEMORY:
//            std::cerr << "GL ERROR: OUT OF MEMORY" << std::endl;
//            break;
//        default:
//            break;
//    }

    setActiveProgram(gl::programId);
    setUniform1i("material.diffuse", 0);
    setUniform1i("material.specular", 1);
//    setUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
    setUniform1f("material.shininess", 32.0f);

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

    // let's try loading in the generated terrain
    float ** terrainMesh = generateTerrain(256, 32);
    // I think we have to do this
    float * flattenedMesh = new float[256 * 256 * 8];
    for (int i = 0; i != 256; ++i) {
        std::memcpy(&flattenedMesh[i * 256 * 8], terrainMesh[i], 256 * 8 * sizeof(float));
    }

    GLuint terrainVAO, terrainVBO;
    glGenVertexArrays(1, &terrainVAO);
    glGenBuffers(1, &terrainVBO);

    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, 256 * 256 * 8 * sizeof(float), flattenedMesh, GL_STATIC_DRAW);

    glBindVertexArray(terrainVAO);
    // terrain position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    // terrain normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // terrain textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint grassDiffuseMap = loadTexture2D(R"(../resources/grass.jpg)");
    GLuint grassSpecularMap = loadTexture2D(R"(../resources/grass.jpg)");

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


    // wireframes
    // TODO add this as some debugging feature

    // main loop
    // TODO add proper input processing
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

        // set up PVM
        glm::mat4 proj = glm::perspective(glm::radians(program.activeCamera.mZoom),
                                          (float) Config::WINDOW_WIDTH / (float) Config::WINDOW_HEIGHT,
                                          Config::ZNEAR,
                                          Config::ZFAR);
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

        int pvmLoc;
        setUniformMat4("PVM", PVM);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

//=============================================================================
        // render cubes
        setActiveProgram(gl::programId);

        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
//        lightColor.x = sin(glfwGetTime() * 2.0f);
//        lightColor.y = sin(glfwGetTime() * 0.7f);
//        lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);

        setUniform3f("viewPos", program.activeCamera.mPosition);

        // directional light
        renderDirectionalLight("dirLight",
                               {-0.2f, -1.0f, -0.3f},
                               {0.60f, 0.60f, 0.60f},
                               {0.4f, 0.4f, 0.4f},
                               {0.5f, 0.5f, 0.5f});

        // point lights
        for (int i = 0; i != 4; ++i) {
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

        // terrain (please work)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassDiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, grassSpecularMap);

        glBindVertexArray(terrainVAO);
        glm::mat4 terrainModel = glm::mat4(1.0f);
        terrainModel = glm::translate(terrainModel, {1.2f, 1.0f, 2.0f});
        PVM = proj * view * terrainModel;

        setUniformMat4("PVM", PVM);
        setUniformMat4("model", terrainModel);

        glDrawArrays(GL_TRIANGLES, 0, 256 * 256 * 3);

//        switch (glGetError()) {
//            case GL_NO_ERROR:
//                break;
//            case GL_INVALID_ENUM:
//                std::cerr << "GL ERROR: INVALID ENUM" << std::endl;
//                break;
//            case GL_INVALID_VALUE:
//                std::cerr << "GL ERROR: INVALID VALUE" << std::endl;
//                break;
//            case GL_INVALID_OPERATION:
//                std::cerr << "GL ERROR: INVALID OPERATION" << std::endl;
//                break;
//            case GL_INVALID_FRAMEBUFFER_OPERATION:
//                std::cerr << "GL ERROR: INVALID FRAMEBUFFER OPERATION" << std::endl;
//                break;
//            case GL_OUT_OF_MEMORY:
//                std::cerr << "GL ERROR: OUT OF MEMORY" << std::endl;
//                break;
//            default:
//                break;
//        }

        // check and call events and swap the buffers
        glfwSwapBuffers(gl::mainWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    freeTerrain(terrainMesh, 256);
    delete[] flattenedMesh;

    glfwTerminate();
    return 0;
}
