//--
//-- RenderSystem.h
//--
#include "../sundile/sundile.h"
#include "../components/Renderer.h"
SYSTEM_DEF_BEGIN(RenderSystem)

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
		for (Shader shader : ShaderRegistry) {
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
							Model model = registry->get<Model>(entity);
							if (registry->has<wireframe>(entity)) {
								glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
								model.Draw(shader);
								glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							}
							else {
								model.Draw(shader);
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

							//-- Draw model
							Mesh model = registry->get<Mesh>(entity);
							if (registry->has<wireframe>(entity)) {
								glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
								model.Draw(shader);
								glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							}
							else {
								model.Draw(shader);
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
							Model model = registry->get<Model>(entity);
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
		SetCamera(rend, ev);
		});

	ev.evw->dispatcher.sink<RenderEvent>().connect<catchRenderEvent>();
}

void terminate(Renderer& rend) {
}

SYSTEM_DEF_END