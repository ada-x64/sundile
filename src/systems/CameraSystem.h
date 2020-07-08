#pragma once
#include "../sundile/EventWrapper.h"
#include "../components/Camera.h"

//test

SYSTEM_DEF_BEGIN(CameraSystem)

void catchInputEvent(const SimInputEvent& ev) {
	//--
	//-- Main button handling
	//--
	ev.registry->view<camera, velocity>().each([&](auto entity, camera& cam, velocity& _vel) {
		//-- Vars
		glm::vec3& vel = _vel.vel;

		//-- Camera Button Registration
		float camspeed = 1.f * float(ev.deltaTime);
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
	});
}

void catchStepEvent(const SimStepEvent& ev) {
	ev.registry->view<camera>().each([&](auto entity, camera& cam) {
		cam.pos += cam.spd;
		cam.target += cam.spd;
		cam.lookat = glm::lookAt(cam.pos, cam.target, cam.up);
	});
}

void init(const SimInitEvent& ev) {
	ev.evw->dispatcher.sink<SimInputEvent>().connect<&catchInputEvent>();
	ev.evw->dispatcher.sink<SimStepEvent>().connect<&catchStepEvent>();

	//dependencies
	ev.registry->on_construct<camera>().connect<&entt::registry::emplace_or_replace<velocity>>();
}

SYSTEM_DEF_END