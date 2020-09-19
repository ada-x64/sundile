//--
//-- RenderSystem.h
//--
#include "../sundile/sundile.h"
#include "../components/Renderer.h"
#include "ModelSystem.h"
BEGIN_SYSTEM(RenderSystem)

	//-- Rendering Shorthands
	namespace {
		void checkError() {
			GLenum error = glGetError();
			if (error) {
				std::cout << "OPENGL ERROR::" << error << std::endl;
				SUNDILE_DEBUG_BREAK
			}
		}

		void UpdateCamera(Renderer& rend, const RenderEvent& ev) {
			Shader passthrough = rend.passthrough;
			SmartRegistry registry = ev.registry;

			//-- Apply camera
			registry->view<camera>().each([=](auto entity, auto& cam) {
				ShaderSystem::use(passthrough);
				int uView = glGetUniformLocation(passthrough, "view");
				glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(cam.mvp));
				checkError();

				int uProj = glGetUniformLocation(passthrough, "projection");
				glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(cam.projection));
				checkError();
				});
		}

		void SetCamera(Renderer& rend, const SimInitEvent& ev) {
			Shader passthrough = rend.passthrough;
			SmartRegistry registry = ev.registry;

			//-- Apply camera
			registry->view<camera>().each([=](auto entity, auto& cam) {
				ShaderSystem::use(passthrough);
				int uView = glGetUniformLocation(passthrough, "view");
				glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(cam.mvp));
				checkError();

				int uProj = glGetUniformLocation(passthrough, "projection");
				glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(cam.projection));
				checkError();
				});

		}

		void RenderVisible(Renderer& rend, const RenderEvent& ev) {
			Shader passthrough = rend.passthrough;
			SmartRegistry registry = ev.registry;

			//-- Render visible models
			for (Shader shader : ShaderSystem::ShaderRegistry) {
				//-- Use current shader.
				ShaderSystem::use(shader);

				//-- Do passthrough (no shader component required)
				if (shader == passthrough) {
					registry->view<visible>().each([=](auto& entity, visible& vis) {
						if (vis.is_visible) {
							if (registry->has<Model>(entity)) {
								//-- Set position
								glm::mat4 mat_model = glm::mat4(1.f);
								if (registry->has<position>(entity)) {
									position& p = registry->get<position>(entity);
									mat_model = glm::translate(mat_model, p.pos);
								}
								ShaderSystem::setMat4(shader, "model", mat_model);

								//-- Draw model
								Model& model = registry->get<Model>(entity);
								if (registry->has<wireframe>(entity)) {
									glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
									ModelSystem::Draw(std::move(model), shader);
									glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
								}
								else {
									ModelSystem::Draw(std::move(model), shader);
								}
							}
							if (registry->has<Mesh>(entity)) {
								//-- Set position
								glm::mat4 mat_model = glm::mat4(1.f);
								if (registry->has<position>(entity)) {
									position& p = registry->get<position>(entity);
									mat_model = glm::translate(mat_model, p.pos);
								}
								ShaderSystem::setMat4(shader, "model", mat_model);

								//-- Draw mesh
								Mesh& mesh = registry->get<Mesh>(entity);
								if (registry->has<wireframe>(entity)) {
									glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
									MeshSystem::Draw(std::move(mesh), shader);
									glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
								}
								else {
									MeshSystem::Draw(std::move(mesh), shader);
								}
							}
							else {
								//is likely a tri, quad, or GUI element
							}
						}
						});
				}
				else {
					//-- Render those with the given shader. (This could probably be made more efficient ?)
					registry->view<visible, Shader>().each([=](auto& entity, auto& vis, auto& _s) {
						if (_s == shader) {
							if (registry->has<Model>(entity)) {
								//-- Set position
								glm::mat4 mat_model = glm::mat4(1.f);
								if (registry->has<position>(entity)) {
									position& p = registry->get<Components::position>(entity);
									mat_model = glm::translate(mat_model, p.pos);
								}
								ShaderSystem::setMat4(shader, "model", mat_model);

								//-- Draw
								Model& model = registry->get<Model>(entity);
								ModelSystem::Draw(std::move(model), shader);
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
	void catchRenderEvent(const RenderEvent& ev) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ev.registry->view<Renderer>().each([&](auto& e, auto& rend) {
			UpdateCamera(rend, ev);
			RenderVisible(rend, ev);
			glBindVertexArray(0);
			});
	}

	void init(const SimInitEvent& ev) {
		//GL, STB
		glEnable(GL_DEPTH_TEST);
		stbi_set_flip_vertically_on_load(true);
		ev.registry->view<Renderer>().each([&](auto& e, auto& rend) {
			//NOTE: This is not being set, causing OpenGL error 1281.
			rend.passthrough = ShaderSystem::init("assets/shaders/passthrough.vert", "assets/shaders/passthrough.frag");
			SetCamera(rend, ev);
			});

		ev.evw->dispatcher.sink<RenderEvent>().connect<catchRenderEvent>();
	}

	void terminate(Renderer& rend) {
	}

END_SYSTEM