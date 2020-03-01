#pragma once
#include <iostream>
#include <cstdio>
#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <stb_image.h>
#include <GLFW/glfw3.h>

class Renderer
{
public:

	Renderer();
	~Renderer() {};

	void init();
	void render();

	//GLFW Settings
	GLFWwindow* window;
	int WIDTH = 640, HEIGHT = 480;

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

	//Camera
	glm::mat4 mat_model = glm::mat4(1.f);
	//glm::mat4 mat_view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -3.f));
	glm::mat4 mat_proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f); //Default camera

	glm::mat4 mat_view = glm::lookAt(
		glm::vec3(0.f,0.f,3.f),	//position
		glm::vec3(0.f,0.f,0.f),	//lookat
		glm::vec3(0.f,1.f,0.f)	//direction
	);
};

