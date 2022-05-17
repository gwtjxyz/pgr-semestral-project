#pragma once

#include <vector>


#include "common.h"
//----------------------------------------------------------------------------------------
/**
 * \file       imported_mesh.h
 * \author     Yury Udavichenka
 * \date       01/05/2022
 * \brief      Mesh generator
 *
 * Generates meshes based on input from the model.h class. This is code mostly grabbed
 * from learnopengl.com with slight adjustments to allow for pick FBO generation.
 *
*/
//----------------------------------------------------------------------------------------
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
    GLuint mVAOpick;
private:
    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;
};



