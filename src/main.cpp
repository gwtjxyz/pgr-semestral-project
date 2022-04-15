#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "callback.h"
#include "utils.h"

GLuint vertexShader = 0, fragmentShader = 0, shaderProgram = 0;
GLFWwindow * window = nullptr;

bool init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(Constants::WINDOW_WIDTH,
                              Constants::WINDOW_HEIGHT,
                              "Evil Tower",
                              nullptr,
                              nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window!\n";
        return false;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!\n";
        return false;
    }

    // tell OpenGL to render its stuff inside the GLFW window
    glViewport(0, 0, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
    // tell GLFW what to use for resizing
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    return true;
}

// loads shaders from files and links them into the program
void loadShaders() {
    vertexShader = createShader(GL_VERTEX_SHADER, R"(..\shaders\vertexShaderSource.vert)");
    fragmentShader = createShader(GL_FRAGMENT_SHADER, R"(..\shaders\fragmentShaderSource.frag)");
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR linking shader program! Log:\n" << infoLog << '\n';
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


int main() {
    if (!init()) {
        glfwTerminate();
        return -1;
    }

    // vertices and their respective buffers
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };

    shaderProgram = glCreateProgram();

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy vertices into a buffer for OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    //load shaderinos
    loadShaders();

    // main loop
    // TODO add proper input processing
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        // we render here
        glUseProgram(shaderProgram);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
