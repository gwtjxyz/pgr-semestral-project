#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "imported_mesh.h"
//----------------------------------------------------------------------------------------
/**
 * \file       model.h
 * \author     Yury Udavichenka
 * \date       01/05/2022
 * \brief      Assimp loader
 *
 * Assimp-based model loader. Very broken and frankly bad, but it gets the job done.
 * And by 'gets the job done' I mean it does in like 20% of cases, the rest of the time
 * I have to manually load in specular and diffuse textures because this thing is
 * extremely dumb. I probably should've written my own version of this (this one is
 * from learnopengl.com and their tutorials) but the class doesn't grade me for how
 * well I can write an Assimp-using class and I couldn't be bothered writing an .OBJ parser.\n
 *
*/
//----------------------------------------------------------------------------------------

/// Data structure/class holding model data for subsequent loading and displaying
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