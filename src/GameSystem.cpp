//--
//-- GameSystem.cpp
//--
#include "./headers/sundile/GameSystem.h"
namespace sundile {
	namespace GameSystem {
		using namespace Components;

		namespace /* Private */ {

			//Todo: Do this properly.
			//template <typename component, typename ...dependencies>
			//void setDependencies(dependencies... args) {
			//
			//	registry.view<component>().each([=](auto entity, auto& comp) {
			//		for (auto arg : args) {
			//			if (!registry.has<typeof(arg)>(entity)) {
			//				registry.emplace<typeof(arg)>(entity);
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
			void setDependencies(Game& game) {
				//Camera
				game.registry.view<camera>().each([&](auto entity, auto& cam) {
					if (!game.registry.has<velocity>(entity)) {
						game.registry.emplace<velocity>(entity, glm::vec3(0.f, 0.f, 0.f));
					}
				});
				//Velocity
				game.registry.view<velocity>().each([&](auto entity, auto& vel) {
					if (!game.registry.has<position>(entity)) {
						game.registry.emplace<position>(entity, glm::vec3(0.f, 0.f, 0.f));
					}
				});
			}

			//Parses GLFW key events and passes appropriate information to other functions.
			void handleInput(const InputEvent& event) {
				//-- Emergency Exit
				if (event.key == GLFW_KEY_ESCAPE) {
					printf("Exiting on Keypress: Esc");
					exit(EXIT_FAILURE);
				}

				//--
				//-- Main button handling
				//-- (eventually, handle this with events)
				/**
				game.registry.view<velocity>().each([&](auto entity, auto& _vel) {
					//-- Vars
					glm::vec3& vel = _vel.vel;

					//-- Camera Button Registration
					if (game.registry.has<camera>(entity)) {
						float camspeed = 1.f * deltaTime;
						camera& cam = game.registry.get<camera>(entity);
						glm::vec3& spd = cam.spd;

						if (action == GLFW_PRESS) {
							switch (key) {
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
						if (action == GLFW_RELEASE) {
							camspeed = -camspeed;
							switch (key) {
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
					/**
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
					/**

					});
				/**/
			}
		}

		void windowTestEvent(const WindowEvent& event) {
			printf("calling windowTestEvent");
		}

		//--
		//-- Initialization
		//-- 
		Game init(SmartEVW evw) {
			// Connect event listeners
			Game game;
			game.evw = evw;
			evw->dispatcher.sink<InputEvent>().connect<&handleInput>();
			evw->dispatcher.sink<WindowEvent>().connect<&windowTestEvent>();

			//Set dependencies for created entites.
			setDependencies(game);
			return game;
		}

		//--
		//-- Main Loop
		//--
		void step(Game& game) {

			//-- Time
			float currentTime = glfwGetTime();
			game.deltaTime = currentTime - game.lastTime;
			game.lastTime = currentTime;

			SmartEVW evw = game.evw;

			//-- Velocity
			game.registry.view<position, velocity>().each([&](auto entity, auto& _pos, auto& _vel) {
				//-- Vars
				glm::vec3& vel = _vel.vel;
				glm::vec3& pos = _pos.pos;

				//-- Camera
				if (game.registry.has<camera>(entity)) {
					//Get cam
					camera& cam = game.registry.get<camera>(entity);
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
					float _fric = cam.fric * game.deltaTime;
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

	}
}
