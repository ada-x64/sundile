//--
//-- Renderer.cpp
//--
#include "./headers/sundile/RenderSystem.h"
namespace sundile {

	namespace RenderSystem {
		using namespace Components;

		//--
		//-- Private functions 
		//--

		//-- Rendering Shorthands
		namespace {

			void UpdateCamera(Renderer& rend) {
				//-- Apply camera
				rend.registry->view<camera>().each([=](auto entity, auto& cam) {
					ShaderSystem::use(rend.passthrough);
					int uView = glGetUniformLocation(rend.passthrough, "view");
					glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(cam.lookat));
					});
			}

			void SetCamera(Renderer& rend) {
				Shader passthrough = rend.passthrough;
				SmartEVW evw = rend.evw;

				//-- Apply camera
				rend.registry->view<camera>().each([=](auto entity, auto& cam) {
					ShaderSystem::use(passthrough);
					int uView = glGetUniformLocation(passthrough, "view");
					glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(cam.lookat));

					int uProj = glGetUniformLocation(passthrough, "projection");
					glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(cam.projection));
					});

			}

			void RenderVisible(Renderer& rend) {
				Shader passthrough = rend.passthrough;
				SmartEVW evw = rend.evw;

				//-- Render visible models
				for (Shader shader : ShaderRegistry) {
					//-- Use current shader.
					ShaderSystem::use(shader);

					//-- Do passthrough (no shader component required)
					if (shader == passthrough) {
						rend.registry->view<visible>().each([=](auto& entity, auto& vis) {
							if (rend.registry->has<Model>(entity)) {
								//-- Set position
								glm::mat4 mat_model = glm::mat4(1.f);
								if (rend.registry->has<position>(entity)) {
									position& p = rend.registry->get<position>(entity);
									mat_model = glm::translate(mat_model, p.pos);
								}
								ShaderSystem::setMat4(shader, "model", mat_model);

								//-- Draw model
								Model model = rend.registry->get<Model>(entity);
								model.Draw(shader);
							}
							else {
								//is likely a tri, quad, or GUI element
							}
							});
					}
					else {
						//-- Render those with the given shader. (This could probably be made more efficient ?)
						rend.registry->view<visible, Shader>().each([=](auto& entity, auto& vis, auto& _s) {
							if (_s == shader) {
								if (rend.registry->has<Model>(entity)) {
									//-- Set position
									glm::mat4 mat_model = glm::mat4(1.f);
									if (rend.registry->has<position>(entity)) {
										position& p = rend.registry->get<Components::position>(entity);
										mat_model = glm::translate(mat_model, p.pos);
									}
									ShaderSystem::setMat4(shader, "model", mat_model);

									//-- Draw
									Model model = rend.registry->get<Model>(entity);
									model.Draw(shader);
								}
								else {
									//is likely a tri, quad, or GUI element
								}
							}
							});
					}
				}
			}
		}

		//-- Event Handling
		namespace {


			void renderEvent(const RenderEvent& rev) {
				//?? it's here if we need it
				/**
				switch (rev.type) {

				}
				/**/
			}
		}

		//--
		//-- Init
		//--
		Renderer init(SmartEVW evw, SmartRegistry registry) {
			Renderer rend;
			rend.evw = evw;
			rend.registry = registry;
			return init(rend);
		}
		Renderer init(Renderer& rend) {
			//GL, STB
			glEnable(GL_DEPTH_TEST);
			stbi_set_flip_vertically_on_load(true);
			SetCamera(rend);

			rend.evw->dispatcher.sink<RenderEvent>().connect<&renderEvent>();

			return rend;
		}

		//--
		//-- Render
		//--
		Renderer render(Renderer& rend) {

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			UpdateCamera(rend);
			RenderVisible(rend);

			glBindVertexArray(0);
			return rend;
		}


		//--
		//-- Terminate 
		//--
		void terminate(Renderer& rend) {
		}
	}
}