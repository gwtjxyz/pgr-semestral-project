#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "windowing.h"
#include "utils.h"

GLuint vertexShader = 0, fragmentShader = 0;

// loads shaders from files and links them into the program
void loadShaders() {
    createShader(GL_VERTEX_SHADER, R"(..\shaders\vertexShaderSource.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(..\shaders\fragmentShaderSource.frag)");
}

void updateUniformColor() {
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(gl::programId, "ourColor");
    glUseProgram(gl::programId);
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}


int main() {
    if (!init()) {
        glfwTerminate();
        return -1;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum # of vertex attributes supported: " << nrAttributes << '\n';

    // vertices and their respective buffers
    float vertices[] = {
            // positions                    // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };

    // load shaderinos
    loadShaders();

    GLuint VAO, VBO;
//    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // uncomment to get wireframes
    // TODO add this as some debugging feature
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // main loop
    // TODO add proper input processing
    while (!glfwWindowShouldClose(gl::mainWindow)) {
        // input
        processInput(gl::mainWindow);
        // we render here
        glUseProgram(gl::programId);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

//        updateUniformColor();

//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) 0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check and call events and swap the buffers
        glfwSwapBuffers(gl::mainWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
