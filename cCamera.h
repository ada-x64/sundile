#pragma once
//--
//-- Camera.h
//--
#include "ComponentHeaders.h"
namespace sundile {
	namespace Components {
		struct camera {
			//-- Matrices
			glm::vec3 up = glm::vec3(0.f, 0.f, 1.f);
			glm::vec3 target = glm::vec3(0.f, 1.f, 0.f);
			glm::vec3 pos = glm::vec3(0.f, 0.f, 3.f);
			glm::mat4 lookat = glm::lookAt(pos, target, up);
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

			//-- Position
			float fric = 0.6f;
			glm::vec3 spd = glm::vec3(0.f);
		};
	}
}