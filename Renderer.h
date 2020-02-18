#pragma once
#include <iostream>
#include <cstdio>
#include <GL/glew.h>
#include <gl/freeglut.h>
#include "Shader.h"
class Renderer
{
public:

	Renderer() {};
	~Renderer() {};

	void init();
	void render();
	void bindVertexArray(const int index, const float* vertices);

	//Objects
	enum VAO {
		vao_tri1,
		vao_tri2,
		vao_num
	};
	enum VBO {
		vbo_tri1,
		vbo_tri2,
		vbo_num
	};

	unsigned int VAOs[vao_num], VBOs[vbo_num];

	Shader* sp_yellow = new Shader("./shaders/passthrough.vert", "./shaders/yellow.frag");
	Shader* sp_orange = new Shader("./shaders/passthrough.vert", "./shaders/orange.frag");


};

