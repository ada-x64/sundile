//--
//-- Camera.h
//--
#pragma once
#include "glm/gtx/rotate_vector.hpp"
#include "../sundile/sundile.h"
#include "Transform.h"

BEGIN_COMPONENT(camera)

	//-- Matrices
	Vec3 pos = Vec3(1.f, 1.f, 1.f);
	Vec3 front = glm::normalize(glm::vec3(-1.f, -1.f, -1.f));
	Vec3 dir = Vec3(0.f, 5.f * glm::quarter_pi<float>(), 0.f);
	Vec3 spd = Vec3(0.f);
	float fric = 0.01f;
	float maxspd = 0.5f;
	Vec2 cursorpos;
	Vec2 cursorpos_prev;
	bool locked = false;

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	transform T =
		glm::rotate(
			glm::translate(glm::mat4(1.f), glm::vec3(-1.f, -1.f, -3.f)),
		dir.y, glm::vec3(0, 1, 0));

END_COMPONENT