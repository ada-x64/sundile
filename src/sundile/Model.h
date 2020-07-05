//--
//-- Model.h
//-- https://learnopengl.com/Model-Loading/Model
//--
#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

namespace sundile {
    class Model {
    public:
        //Data
        std::vector<Texture> textures_loaded;
        std::string directory;

        //Functions
        Model(std::string const& path)
        {
            loadModel(path);
        }
        void Draw(Shader shader);

    private:
        //Model Data
        std::vector<Mesh> meshes;

        //Functions
        void loadModel(std::string const& path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    };
}
#endif