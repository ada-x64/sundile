//--
//-- Systems.cpp
//--
#include "Systems.h"
namespace sundile {
	using namespace Components;
	//--
	//-- Dependencies
	//-- 

	//Todo: Do this properly.
	//template <typename component, typename ...dependencies>
	//void setDependencies(dependencies... args) {
	//
	//	registry.view<component>().each([=](auto entity, auto& comp) {
	//		for (auto arg : args) {
	//			if (!registry.has<typeof(arg)>(entity)) {
	//				registry.assign<typeof(arg)>(entity);
	//			}
	//		}
	//	});
	//}
	//
	//template <typename component, typename ...dependencies>
	//void connectDependencies() {
	//	registry.on_construct<component>().connect(&setDependencies<...dependencies>);
	//}

	//-- Rough dependencies function. Find a way to replace this eventually.
	void Systems::setDependencies() {
		//Camera
		registry.view<camera>().each([=](auto entity, auto& cam) {
			if (!registry.has<velocity>(entity)) {
				registry.assign<velocity>(entity, glm::vec3(0.f, 0.f, 0.f));
			}
			});
		//Velocity
		registry.view<velocity>().each([=](auto entity, auto& vel) {
			if (!registry.has<position>(entity)) {
				registry.assign<position>(entity, glm::vec3(0.f, 0.f, 0.f));
			}
			});
	}


	//--
	//-- Step
	//-- 

	//Step function - Called every frame
	void Systems::step() {
		//-- Time
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//-- Velocity
		registry.view<position, velocity>().each([](auto entity, auto& _pos, auto& _vel) {
			//-- Vars
			glm::vec3& vel = _vel.vel;
			glm::vec3& pos = _pos.pos;

			//-- Camera
			if (registry.has<camera>(entity)) {
				//Get cam
				camera& cam = registry.get<camera>(entity);
				glm::vec3& spd = cam.spd;
				vel += spd;

				//Update position
				glm::vec3 left = glm::normalize(glm::cross(cam.target, cam.up));
				auto res = left * vel;

				cam.pos.x += res.x;
				cam.pos.y += vel.y;
				cam.pos.z += vel.z;
				cam.target = cam.pos + glm::vec3(0.f, 1.f, -3.f);

				//Do friction
				float _fric = cam.fric * deltaTime;
				vel.x -= _fric * Utility::signum(vel.x);
				if (abs(vel.x) < _fric)
					vel.x = 0;
				vel.y -= _fric * Utility::signum(vel.y);
				if (abs(vel.y) < _fric)
					vel.y = 0;
				vel.z -= _fric * Utility::signum(vel.z);
				if (abs(vel.z) < _fric)
					vel.z = 0;


				//Modify lookat
				cam.lookat = glm::lookAt(cam.pos, cam.target, cam.up);
			}

			//-- General
			else {
				//replace this: phys.pos += phys.vel;
				pos += vel;
			}
			});
	}


	//--
	//-- Input
	//-- 

	// Connects event listeners to dispatcher.
	void Systems::connectListeners() {
		dispatcher.sink<keyEvent>().connect<&Systems::handleKeyEvents>();
	}

	//Parses GLFW key events and passes appropriate information to other functions.
	Systems::keyEvent Systems::handleKeyEvents(const keyEvent& event) {

		//-- Emergency Exit
		if (event.key == GLFW_KEY_ESCAPE) {
			windowShouldClose = true;
		}

		//--
		//-- Main button handling
		//-- (eventually, handle this with events)
		registry.view<velocity>().each([=](auto entity, auto& _vel) {
			//-- Vars
			glm::vec3& vel = _vel.vel;

			//-- Camera Button Registration
			if (registry.has<camera>(entity)) {
				float camspeed = 1.f * deltaTime;
				camera& cam = registry.get<camera>(entity);
				glm::vec3& spd = cam.spd;

				if (event.action == GLFW_PRESS) {
					switch (event.key) {
					case(GLFW_KEY_A):
						spd.x = camspeed;
						break;
					case(GLFW_KEY_D):
						spd.x = -camspeed;
						break;
					case(GLFW_KEY_W):
						spd.y = -camspeed;
						break;
					case(GLFW_KEY_S):
						spd.y = camspeed;
						break;
					case(GLFW_KEY_Q):
						spd.z = camspeed;
						break;
					case(GLFW_KEY_E):
						spd.z = -camspeed;
						break;
					};
				}
				if (event.action == GLFW_RELEASE) {
					camspeed = -camspeed;
					switch (event.key) {
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

			//-- Controllable button registration
			if (registry.has<controllable>(entity)) {
				const float mvspd = 0.05f;
				if (event.action == GLFW_PRESS) {
					if (event.key == GLFW_KEY_UP)
						vel.y -= mvspd;
					if (event.key == GLFW_KEY_LEFT)
						vel.x += mvspd;
					if (event.key == GLFW_KEY_DOWN)
						vel.y += mvspd;
					if (event.key == GLFW_KEY_RIGHT)
						vel.x -= mvspd;
				}
				else if (event.action == GLFW_RELEASE) {
					if (event.key == GLFW_KEY_UP)
						vel.y -= -mvspd;
					if (event.key == GLFW_KEY_LEFT)
						vel.x += -mvspd;
					if (event.key == GLFW_KEY_DOWN)
						vel.y += -mvspd;
					if (event.key == GLFW_KEY_RIGHT)
						vel.x -= -mvspd;
				}
			}

			});

		return event;
	}

}
