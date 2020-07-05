#pragma once
#include "../sundile/SimSystem.h"
#include "../components/Camera.h"

//test

SYSTEM_DEF_BEGIN(CameraSystem)
inline SmartSim sim; 

void catchInputEvent(const SimInputEvent& ev) {
	//--
	//-- Main button handling
	//-- (eventually, handle this with events)
	/**/
	sim->registry->view<velocity>().each([&](auto entity, auto& _vel) {
		//-- Vars
		glm::vec3& vel = _vel.vel;

		//-- Camera Button Registration
		if (sim->registry->has<camera>(entity)) {
			float camspeed = 1.f * float(ev.deltaTime);
			camera& cam = sim->registry->get<camera>(entity);
			glm::vec3& spd = cam.spd;

			if (ev.action == GLFW_PRESS) {
				switch (ev.key) {
				case(GLFW_KEY_A):
					spd.x = -camspeed;
					break;
				case(GLFW_KEY_D):
					spd.x = camspeed;
					break;
				case(GLFW_KEY_W):
					spd.y = camspeed;
					break;
				case(GLFW_KEY_S):
					spd.y = -camspeed;
					break;
				case(GLFW_KEY_Q):
					spd.z = -camspeed;
					break;
				case(GLFW_KEY_E):
					spd.z = camspeed;
					break;
				};
			}
			if (ev.action == GLFW_RELEASE) {
				camspeed = -camspeed;
				switch (ev.key) {
				case(GLFW_KEY_W):
				case(GLFW_KEY_S):
					spd.y = 0;
					break;
				case(GLFW_KEY_A):
				case(GLFW_KEY_D):
					spd.x = 0;
					break;
				case(GLFW_KEY_Q):
				case(GLFW_KEY_E):
					spd.z = 0;
					break;
				};
			}
		}
		});
}

void init(SmartSim _sim) {
	sim = _sim;
	sim->evw->dispatcher.sink<SimInputEvent>().connect<&catchInputEvent>();
}
SYSTEM_DEF_END