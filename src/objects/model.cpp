#include "model.h"
#include "utils.h"

#include <iostream>


void Model::draw(GLuint shaderId) {
    for (unsigned int i = 0; i != mMeshes.size(); ++i)
        mMeshes[i].draw(shaderId);
}

void Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR ASSIMP " << import.GetErrorString() << '\n';
        return;
    }
    mDirectory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene) {
    // process all the node's meshes, if present
    for (unsigned int i = 0; i != node->mNumMeshes; ++i) {
        aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.push_back(processMesh(mesh, scene));
    }
    // process all their children, if present
    for (unsigned int i = 0; i != node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

ImportedMesh Model::processMesh(aiMesh * mesh, const aiScene * scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i != mesh->mNumVertices; ++i) {
        Vertex vertex;
        // process vertex positions, normals, texture coords
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0]) { // check if textures exist
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i != mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j != face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    // process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                                aiTextureType_DIFFUSE,
                                                                "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,
                                                                 aiTextureType_SPECULAR,
                                                                 "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return ImportedMesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i != mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.id = loadTextureFromFile(str.C_Str(), mDirectory);
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
    }
    return textures;
}