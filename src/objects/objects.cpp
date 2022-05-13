#include "objects.h"
#include "utils.h"

#include <vector>
#include <string>

#include "render.h"

Skybox loadSkybox() {
    loadSkyboxShaders();
    Skybox skybox{};

    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    std::vector<std::string> skyboxFaces {
            R"(../resources/skybox/day/right.jpg)",
            R"(../resources/skybox/day/left.jpg)",
            R"(../resources/skybox/day/top.jpg)",
            R"(../resources/skybox/day/bottom.jpg)",
            R"(../resources/skybox/day/front.jpg)",
            R"(../resources/skybox/day/back.jpg)"
    };
    skybox.texture = loadCubemap(skyboxFaces);
    skybox.cloudsTexture = loadTexture2D(R"(../resources/clouds.jpg)");

    glGenVertexArrays(1, &skybox.VAO);
    glGenBuffers(1, &skybox.VBO);

    glBindVertexArray(skybox.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, skybox.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    return skybox;
}

Model loadBackpack() {
    Model backpackModel(R"(../resources/models/backpack/backpack.obj)");
    return backpackModel;
}

Image loadLogo() {
    Image logo{};
    float logoVertices[] = {
            -1.0f,  0.15f, 0.0f, 0.0f, 1.0f,
            -1.0f, -0.15f, 0.0f, 0.0f, 0.0f,
            1.0f,  0.15f, 0.0f, 3.0f, 1.0f,
            1.0f, -0.15f, 0.0f, 3.0f, 0.0f
    };

    logo.texture = loadTexture2D(R"(../resources/opengl-logo.png)");

    glGenVertexArrays(1, &logo.VAO);
    glGenBuffers(1, &logo.VBO);

    glBindVertexArray(logo.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, logo.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(logoVertices), logoVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    logo.position = glm::vec3(0.0f, 0.0f, 0.0f);

    return logo;
}

// TODO texture animation