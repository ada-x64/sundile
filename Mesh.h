//--
//-- Mesh.h
//-- https://learnopengl.com/Model-Loading/Mesh
//--
#pragma once
#ifndef MESH_H
#define MESH_H
#include "Shader.h"

namespace sundile {

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture {
        unsigned int id;
        //"specular" or "diffuse"
        std::string type;
        //Filepath
        std::string path;
    };

    class Mesh {

    public:
        //Mesh Data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        //Functions
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader s);

    private:
        //Render data
        unsigned int VAO, VBO, EBO;
        //Functions
        void setupMesh();
    };
}
#endif