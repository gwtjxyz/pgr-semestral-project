#include "imported_mesh.h"

#include <utility>

#include "program.h"
#include "utils.h"

ImportedMesh::ImportedMesh(std::vector<Vertex> vertices,
                           std::vector<GLuint> indices,
                           std::vector<Texture> textures) {
    this->mVertices = std::move(vertices);
    this->mIndices = std::move(indices);
    this->mTextures = std::move(textures);

    setupMesh();
}

void ImportedMesh::setupMesh() {
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER,
                 mVertices.size() * sizeof(Vertex),
                 &mVertices[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mIndices.size() * sizeof(GLuint),
                 &mIndices[0],
                 GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *) 0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *) offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *) offsetof(Vertex, texCoords));

    // create VAO for picking
    glGenVertexArrays(1, &mVAOpick);
    // bind the same stuff to it
    glBindVertexArray(mVAOpick);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    // enable position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *) 0);

    glBindVertexArray(0);
}

void ImportedMesh::draw(GLuint shaderId) {
    setActiveProgram(shaderId);
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for (unsigned int i = 0; i != mTextures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = mTextures[i].type;
        if (name == Config::TEXTURE_DIFFUSE)
            number = std::to_string(diffuseNr++);
        else if (name == Config::TEXTURE_SPECULAR)
            number = std::to_string(specularNr++);

        setUniform1f(("materials[" + number + "]" + name).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}