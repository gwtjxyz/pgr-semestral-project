#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "config.h"
#include "windowing.h"
#include "utils.h"
#include "render.h"

// loads shaders from files and links them into the program

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

    // vertices and their respective buffers
    float vertices[] = {
            // positions                        // colors                       // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    // load shaderinos
    createShader(GL_VERTEX_SHADER, R"(..\shaders\vertexShaderSource.vert)");
    createShader(GL_FRAGMENT_SHADER, R"(..\shaders\fragmentShaderSource.frag)");

    // texture memes
    glUseProgram(gl::programId);        // apparently you have to call this before setting textures, who knew
    stbi_set_flip_vertically_on_load(true);
    GLuint texture1 = loadTexture2D(R"(..\resources\container.jpg)", GL_RGB);
    GLuint texture2 = loadTexture2D(R"(..\resources\awesomeface.png)", GL_RGBA);

    GLint test1 = glGetUniformLocation(gl::programId, "texture1");
    GLint test2 = glGetUniformLocation(gl::programId, "texture2");

    glUniform1i(test1, 0);
    glUniform1i(test2, 1);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // uncomment to get wireframes
    // TODO add this as some debugging feature
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // main loop
    // TODO add proper input processing
    while (!glfwWindowShouldClose(gl::mainWindow)) {
        // input
        processInput(gl::mainWindow);
        // we render here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int data;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glUseProgram(gl::programId);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
