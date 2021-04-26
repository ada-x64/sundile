//--
//-- RenderSystem.h
//--
#include "ModelSystem.h"
#include "CameraSystem.h"
#ifndef SUNDILE_RENDERER_H
#define SUNDILE_RENDERER_H

SYSTEM(RenderSystem)
	static Renderer* currentRenderer;

	//-- Rendering Shorthands
	namespace {
		void checkError() {
			GLenum error = glGetError();
			if (error) {
				std::cout << "OPENGL ERROR::" << error << std::endl;
				SUNDILE_DEBUG_BREAK
			}
		}

		void UpdateCamera(RenderEvent<SmartScene>& ev) {
			auto& scene = ev.member;
			auto& rend = scene->renderer;
			auto& registry = scene->registry;
			Shader defaultShader = rend.defaultShader;

			//-- Apply camera
			registry->view<Camera>().each([=](auto entity, auto& cam) {
				ShaderSystem::use(defaultShader);
				int uView = glGetUniformLocation(defaultShader, "view");
				glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(cam.T));
				checkError();

				int uProj = glGetUniformLocation(defaultShader, "projection");
				glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(cam.projection));
				checkError();
				});
		}

		void SetCamera(RenderEvent<SmartScene>& ev) {
			auto& scene = ev.member;
			auto& rend = scene->renderer;
			auto& registry = scene->registry;
			Shader defaultShader = rend.defaultShader;
			Camera cam = rend.defaultCamera;

			//-- Apply camera
			ShaderSystem::use(defaultShader);
			int uView = glGetUniformLocation(defaultShader, "view");
			glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(cam.T));
			checkError();

			int uProj = glGetUniformLocation(defaultShader, "projection");
			glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(cam.projection));
			checkError();

		}

		void RenderVisible(RenderEvent<SmartScene>& ev) {
			auto& scene = ev.member;
			auto& rend = scene->renderer;
			auto& registry = scene->registry;
			Shader defaultShader = rend.defaultShader;

			//-- Render visible models
			for (Shader shader : ShaderSystem::ShaderRegistry) {
				//-- Use current shader.
				ShaderSystem::use(shader);

				//-- Do passthrough (no shader component required)
				if (shader == defaultShader) {
					registry->view<Model>().each([=](auto entity, auto& model) {
						if (model.is_visible) {
							//\todo: add a component which contains a vector of meshes and modles.
							//get that component and loop over its contents.
							//this allows for more than one mesh/model.
							if (registry->has<Model>(entity)) {
								//-- Set transform
								ShaderSystem::setMat4(shader, "model", model.transform);

								//-- Draw model
								if (model.is_wireframe) {
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
								ShaderSystem::setMat4(shader, "model", model.transform);

								//-- Draw mesh
								Mesh& mesh = registry->get<Mesh>(entity);
								if (mesh.is_wireframe) {
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
					registry->view<Model, Shader>().each([=](auto& entity, auto& model, auto& _s) {
						if (_s == shader) {
							if (registry->has<Model>(entity)) {
								//-- Set position
								ShaderSystem::setMat4(shader, "model", model.transform);

								//-- Draw
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
	void catchRenderEvent(RenderEvent<SmartScene>& ev) {

		auto& scene = ev.member;
		auto& rend = scene->renderer;
		auto& registry = scene->registry;
		Shader defaultShader = rend.defaultShader;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentRenderer = &rend;
		if (!rend.initialized) {
			SetCamera(ev);
		}
		UpdateCamera(ev);
		RenderVisible(ev);
		glBindVertexArray(0);
	}

	//-- public API
	Renderer create(fs::path vert = asset_directory + "/shaders/passthrough.vert", fs::path frag = asset_directory + "/shaders/passthrough.frag", Vec2 size = { 1920,1080 }) {
		Renderer r;
		r.defaultShader = ShaderSystem::create(vert, frag);
		r.size = size;
		checkError();
		return r;
	}

	void setSizePos(Renderer rend, Vec4 vec) {
		glViewport(vec.x, vec.y, vec.z, vec.w);
		rend.size = { vec.z, vec.w };
		rend.defaultCamera.size = { vec.z, vec.w };
		auto& cam = rend.defaultCamera;
		cam.projection = glm::perspective(cam.fovy, cam.size.x / cam.size.y, cam.renderNear, cam.renderFar);
	}


	/**
	void gui(const guiMeta& meta) {
		using namespace ImGui;
		Renderer* c = meta_cast<Renderer>(meta);
		DragFloat2("Position", c->pos);
	}
	/**/

	void init(const InitEvent& ev) {
		//GL, STB
		glEnable(GL_DEPTH_TEST);
		stbi_set_flip_vertically_on_load(true);

		ev.evw->dispatcher.sink<RenderEvent<SmartScene>>().connect<catchRenderEvent>();
		//defineGui<Renderer>(gui);
	}

	void terminate(Renderer& rend) {
	}

END_SYSTEM

#endif