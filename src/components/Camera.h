#pragma once
//--
//-- Camera.h
//--

//test

// #include "../sundile/Components.h"
#include "../components/Physics.h"
#include "glm/gtx/rotate_vector.hpp"

COMPONENT_DEF_BEGIN(camera)

//-- Matrices
glm::vec3 pos = glm::vec3(1.f, 1.f, 1.f);
glm::vec3 front = glm::normalize(glm::vec3(-1.f, -1.f, -1.f));
glm::vec3 dir = glm::vec3(0.f, 5.f * glm::quarter_pi<float>(), 0.f);
//glm::mat4 model = glm::lookAt(pos, pos+front, up);
glm::mat4 model =
	glm::rotate(
		glm::translate(glm::mat4(1.f), glm::vec3(-1.f, -1.f, -3.f)),
	dir.y, glm::vec3(0, 1, 0));

glm::mat4 view = glm::mat4(1.f);
glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
glm::mat4 mvp = glm::mat4(1.f);

//-- Position
float fric = 0.01f;
float maxspd = 0.5f;
glm::vec3 spd = glm::vec3(0.f);
glm::vec2 cursorpos;
glm::vec2 cursorpos_prev;

COMPONENT_DEF_END