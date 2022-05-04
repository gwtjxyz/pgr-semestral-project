#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "config.h"
#include "windowing.h"
#include "utils.h"

// loads shaders from files and links them into the program

void loadMainShaders() {
    setActiveProgram(gl::programId);
    createShader(GL_VERTEX_SHADER, R"(../shaders/vertexShaderSource.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/fragmentShaderSource.frag)");

}

void loadLightingShaders() {
    setActiveProgram(gl::lightingId);
    createShader(GL_VERTEX_SHADER, R"(../shaders/vertexShaderLightSrc.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(../shaders/fragmentShaderLightSrc.frag)");
}

int main() {
    if (!init()) {
        glfwTerminate();
        return -1;
    }

    // vertices and their respective buffers
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    // load shaderinos
    loadMainShaders();

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // lighting - light source creation
    loadLightingShaders();

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO data already has the needed data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


    // wireframes
    // TODO add this as some debugging feature
    if (Config::ENABLE_DEBUG)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set up PVM
        glm::mat4 proj = glm::perspective(glm::radians(program.activeCamera.mZoom),
                                          (float) Config::WINDOW_WIDTH / (float) Config::WINDOW_HEIGHT,
                                          Config::ZNEAR,
                                          Config::ZFAR);
        glm::mat4 view = program.activeCamera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 PVM;

        // render light
        setActiveProgram(gl::lightingId);
        glUseProgram(program.activeId);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        PVM = proj * view * model;

        int pvmLoc = glGetUniformLocation(program.activeId, "PVM");
        glUniformMatrix4fv(pvmLoc, 1, GL_FALSE, glm::value_ptr(PVM));
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // render cubes
        setActiveProgram(gl::programId);
        glUseProgram(program.activeId);
        glUniform3f(glGetUniformLocation(program.activeId, "objectColor"), 1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(program.activeId, "lightColor"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(program.activeId, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(program.activeId, "viewPos"),
                    program.activeCamera.mPosition.x,
                    program.activeCamera.mPosition.y,
                    program.activeCamera.mPosition.z);

        // draw one cube only
        glBindVertexArray(VAO);
        model = glm::mat4(1.0f);
        PVM = proj * view * model;
        pvmLoc = glGetUniformLocation(program.activeId, "PVM");
        glUniformMatrix4fv(pvmLoc, 1, GL_FALSE, glm::value_ptr(PVM));
        pvmLoc = glGetUniformLocation(program.activeId, "model");
        glUniformMatrix4fv(pvmLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
/*        for (unsigned int i = 1; i != 10; ++i) {
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (float) i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            PVM = proj * view * model;

            int pvmLoc = glGetUniformLocation(gl::programId, "PVM");
            glUniformMatrix4fv(pvmLoc, 1, GL_FALSE, glm::value_ptr(PVM));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/

        // check and call events and swap the buffers
        glfwSwapBuffers(gl::mainWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
