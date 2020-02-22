#pragma once
#include <iostream>
#include <cstdio>
#include <GL/glew.h>
#include <gl/freeglut.h>
#include "Shader.h"
#include <stb_image.h>

class Renderer
{
public:

	Renderer() {};
	~Renderer() {};

	void init();
	void render();

	//Objects
	enum VAO {
		vao0,
		vao_num
	};
	enum VBO {
		vbo0,
		vbo_num
	};
	enum EBO {
		ebo0,
		ebo_num
	};

	unsigned int VAOs[vao_num], VBOs[vbo_num], EBOs[ebo_num];

	Shader* sp_yellow = new Shader("./shaders/passthrough.vert", "./shaders/yellow.frag");
	Shader* sp_orange = new Shader("./shaders/passthrough.vert", "./shaders/orange.frag");
	Shader* sp_passthrough = new Shader("./shaders/passthrough.vert", "./shaders/passthrough.frag");
	
	unsigned int texture;

};

