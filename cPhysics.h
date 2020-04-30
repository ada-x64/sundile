#pragma once
//--
//-- Physics.h
//--
#include "ComponentHeaders.h"
namespace sundile {
	namespace Components {
		//vec3 pos {x, y, z}
		struct position {
			glm::vec3 pos;
		};

		//Gives euler angles (x, y, z)
		//Does so by using pitch, yaw, and roll.
		struct orientation {
			//x => pitch, y => yaw, z => roll
			glm::vec3 pyr = glm::vec3(0.f);
			//Change in PYR
			glm::vec3 d_pyr = glm::vec3(0.f);
			//Normalized direction given PYR. Determined by step().
			glm::vec3 dir = glm::vec3(0.f);
			//Friction
			glm::vec3 friction = glm::vec3(0.5f);

			//Step
			glm::vec3 step(float deltaTime) {
				//Update PYR
				glm::vec3 dfric = friction * deltaTime;
				pyr += d_pyr;
				if (abs(d_pyr.x) >= dfric.x) {
					d_pyr.x -= glm::sign(d_pyr.x) * dfric.x;
				}
				else d_pyr.x = 0.f;
				if (abs(d_pyr.y) >= dfric.y) {
					d_pyr.y -= glm::sign(d_pyr.y) * dfric.y;
				}
				else d_pyr.y = 0.f;
				if (abs(d_pyr.z) >= dfric.z) {
					d_pyr.z -= glm::sign(d_pyr.z) * dfric.z;
				}
				else d_pyr.z = 0.f;

				//Update dir
				this->dir = glm::normalize(
					glm::vec3(
						cos(glm::radians(pyr.y)) * cos(glm::radians(pyr.x)),
						sin(glm::radians(pyr.x)),
						sin(glm::radians(pyr.y)) * cos(glm::radians(pyr.x))
					)
				);
				return dir;
			}
			_dep<position> __dep;
		};


		// Without Orientation:
		//	vec3 vel {dx, dy, dz}
		// With Orientation:
		//	vec3 vel {left/right, front/back, up/down}
		struct velocity {
			glm::vec3 vel;
			_dep<position> __dep;
		};

		//Velocity, position, orientation all in one
		struct physics {

		};
	}
}
