//--
//-- Renderer.cpp
//--
#include "Renderer.h"
namespace sundile {
	namespace Renderer {
		using namespace Components;
		using namespace Systems;

		//--
		//-- Init
		//--
		void init() {
			//Set up
			glEnable(GL_DEPTH_TEST);
			stbi_set_flip_vertically_on_load(true);
			Window::init();
			passthrough = ShaderSystem::init("./shaders/passthrough.vert", "./shaders/passthrough.frag");
		}

		//--
		//-- Render
		//--
		void render() {
			//-- Clear screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//-- Apply camera
			registry.view<Components::camera>().each([=](auto entity, auto& cam) {
				ShaderSystem::use(passthrough);
				int uView = glGetUniformLocation(passthrough.ID, "view");
				glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(cam.lookat));

				//TODO: Move this to initialization; only update when needed.
				int uProj = glGetUniformLocation(passthrough.ID, "projection");
				glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(cam.projection));
				});


			//-- Render visible models
			for (Shader shader : ShadersInUse) {
				//-- Use current shader.
				ShaderSystem::use(shader);

				//-- Do passthrough (no shader component required)
				if (shader == passthrough) {
					registry.view<Components::visible>().each([=](auto& entity, auto& vis) {
						if (registry.has<Model>(entity)) {
							//-- Set position
							glm::mat4 mat_model = glm::mat4(1.f);
							if (registry.has<position>(entity)) {
								position& p = registry.get<position>(entity);
								mat_model = glm::translate(mat_model, p.pos);
							}
							ShaderSystem::setMat4(shader,"model", mat_model);

							//-- Draw model
							Model model = registry.get<Model>(entity);
							model.Draw(shader);
						}
						else {
							//is likely a tri, quad, or GUI element
						}
						});
				}
				else {
					//-- Render those with the given shader. (This could probably be made more efficient ?)
					registry.view<Components::visible, Shader>().each([=](auto& entity, auto& vis, auto& _s) {
						if (_s == shader) {
							if (registry.has<Model>(entity)) {
								//-- Set position
								glm::mat4 mat_model = glm::mat4(1.f);
								if (registry.has<position>(entity)) {
									position& p = registry.get<Components::position>(entity);
									mat_model = glm::translate(mat_model, p.pos);
								}
								ShaderSystem::setMat4(shader,"model", mat_model);

								//-- Draw
								Model model = registry.get<Model>(entity);
								model.Draw(shader);
							}
							else {
								//is likely a tri, quad, or GUI element
							}
						}
						});
				}
			}
			glBindVertexArray(0);
		}
	}
}