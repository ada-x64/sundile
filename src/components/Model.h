//--
//-- Model.h
//-- https://learnopengl.com/Model-Loading/Model
//--
#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

BEGIN_COMPONENT(Model)
    //Data
    std::vector<Texture> textures_loaded;
    std::string directory;
    std::vector<Mesh> meshes;

    
END_COMPONENT
#endif