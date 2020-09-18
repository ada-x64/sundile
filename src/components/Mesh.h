//--
//-- Mesh.h
//-- https://learnopengl.com/Model-Loading/Mesh
//--
#pragma once
#ifndef MESH_H
#define MESH_H
#include "../sundile/sundile.h"
#include "Shader.h"

BEGIN_COMPONENT(Vertex)

    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    NO_GUI

END_COMPONENT

BEGIN_COMPONENT(Texture)

    unsigned int id;
    //"specular" or "diffuse"
    std::string type;
    std::string path;
    NO_GUI

END_COMPONENT

BEGIN_COMPONENT(Mesh)

    Mesh(auto vertices, auto indices, auto textures) : vertices(vertices), indices(indices), textures(textures) {};
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    //Render data
    unsigned int VAO, VBO, EBO;

    NO_GUI

END_COMPONENT
#endif