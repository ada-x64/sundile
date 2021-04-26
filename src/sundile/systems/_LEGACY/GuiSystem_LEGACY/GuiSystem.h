//--
//-- GuiSystem.h
//--
#pragma once

#ifndef GUI_H
#define GUI_H

#include "../EventSystem/EventSystem.h"

#include "GuiMeta.h"
#include "GuiTypes.h"
#include "GuiFrontEnd.h"

/*
\\TODO:
	Use window time
*/


#ifndef SUNDILE_EXPORT
SYSTEM(GuiSystem)

	//-- Events
	//back end - to be called according to a timer (every second?)

	listEntity createListEntity(entt::entity e) {
		//Find or create guiEntity
		listEntity guiEntity;
		for (auto _e : entityList) {
			if (_e.entity == e) {
				guiEntity = _e;
				break;
			}
		}
		if (guiEntity.entity == entt::null) {
			std::string name = "Entity #" + std::to_string(int(e));
			guiEntity.entity = e;
			guiEntity.name = ("Entity #" + std::to_string(int(e)));
		}
		return guiEntity;
	}

	void refreshEntities(ChangeEvent<SmartScene>& ev) {
		//for every entity:
		//	get the entity's component IDs.
		//	if there is an associated listEntity then copy it.
		//	otherwise create a new one.
		//	for every component ID:
		//		if the ID matches a captured ID:
		//			find or create a listComponent

		entityList.clear();
		auto registry = SceneSystem::currentScene->registry;
		registry->each([=](entt::entity e) {

		listEntity guiEntity = createListEntity(e);

		//Find or create component
		if (!registry->orphan(e)) {
			registry->visit(e, [&](const entt::id_type id) {
				for (auto& meta : ev.member->guiMetas) {
					if (meta.id == id) {
						//Find or create listComponent
						listComponent component;
						for (listComponent& c : guiEntity.componentList) {
							if (id == c.index.id) {
								component = c;
								break;
							}
						}

						//Initialize empty listComponent
						if (component.meta.id == -1) {
							//set meta
							component.meta = meta;

							//get guiIndex
							for (guiIndex& i : guiIndices) {
								if (i.id == component.meta.id) {
									component.index = i;
									break;
								}
							}
						}

						//Replace or push_back
						bool set = false;
						for (auto& _c : guiEntity.componentList) {
							if (_c.index.id == component.index.id) {
								_c = component;
								set = true;
								break;
							}
						}
						if (!set) {
							guiEntity.componentList.push_back(component);
						}

						break;
					}
				}
				});
		}

		//Add guiEntity to entityList
		bool set = false;
		for (auto& _e : entityList) {
			if (_e.entity == e) {
				_e = guiEntity;
				set = true;
				break;
			}
		}
		if (!set) {
			entityList.push_back(guiEntity);
		}
		});
	}

	void render() {
		checkContext();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Render all GUI elements here
		auto c = getPrimaryContainer();
		c->renderFunc(*c);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void windowInit(const WindowInitEvent& ev) {
		auto win = guiRegistry.create();
		guiRegistry.emplace<windowID>(win, ev.id);

		ImGui_ImplGlfw_InitForOpenGL(ev.window, true);
		ImGui_ImplOpenGL3_Init(SUNDILE_GLSL_VERSION);

		if (currentWindow == -1) {
			currentWindow = ev.id;

			Vec2 windowSize = getWindowSizeByID(getEVWByID(currentEVW), currentWindow);
			float ww = windowSize.x;
			float wh = windowSize.y;
			float viewport_w = 800;
			float viewport_h = 600;
			float viewport_x = ww / 2 - viewport_w / 2;
			float viewport_y = wh / 2 - viewport_h / 2;
			glViewport(viewport_x, viewport_y, viewport_w, viewport_h);
		}
	}
	void init(SmartEVW evw) {
		//Initalize
		IMGUI_CHECKVERSION();
		checkContext();
		ImGui::StyleColorsDark();

		//Setup font
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* tex_pixels = NULL;
		int tex_w, tex_h;
		io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);

		//do event stuff
		currentEVW = evw->id;
		evw->dispatcher.sink<WindowInitEvent>().connect<GuiSystem::windowInit>();
		evw->dispatcher.sink<RenderGuiEvent>().connect<&render>();
		evw->dispatcher.sink<TerminateEvent>().connect<&terminate>();
		evw->dispatcher.sink<ChangeEvent<SmartScene>>().connect<&refreshEntities>(); //\TODO: Replace this with something that updates entities only when a change is detected

		primaryGuiEntity = guiRegistry.create();
		guiRegistry.emplace<guiContainer>(primaryGuiEntity, "primary container",
			[](guiContainer& e) -> void {
				auto evw = getEVWByID(currentEVW);
				ImVec2 ImWindowSize = getWindowSizeByID(evw, currentWindow);
				stateSetter(evw, e, ImWindowSize);
				mainMenu(evw, e, ImWindowSize);
				stateRouter(evw, e, ImWindowSize);
			}
		);
		initGuiFrontend(evw);
	}
END_SYSTEM
#endif 
#endif