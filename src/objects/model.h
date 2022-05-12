#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "imported_mesh.h"

class Model {
public:
    Model(const char * path) {
        loadModel(path);
    }
     void draw(GLuint shaderId);
private:
    void loadModel(std::string path);
    void processNode(aiNode * node, const aiScene * scene);
    ImportedMesh processMesh(aiMesh * mesh, const aiScene * scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial * mat,
                                              aiTextureType type,
                                              std::string typeName);
public:
    std::vector<ImportedMesh> mMeshes;
private:
    std::vector<Texture> mLoadedTextures;
    std::string mDirectory;
};



