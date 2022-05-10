#pragma once

#include <vector>


#include "common.h"

class ImportedMesh {
public:
    ImportedMesh(std::vector<Vertex> vetices,
                 std::vector<GLuint> indices,
                 std::vector<Texture> textures);
    void draw(GLuint shaderId);
private:
    void setupMesh();
public:
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    std::vector<Texture> mTextures;
private:
    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;
};



