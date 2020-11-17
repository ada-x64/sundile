#ifndef GUI_FRONTEND_H
#define GUI_FRONTEND_H

#include "GuiTypes.h"
#include "GuiUtility.h"

namespace sundile::GuiSystem {
	/*

	# TODO:

	## Current bug:

	Currently I think this is failing becase entitiesSelected and componentsSelected
	are static and aren't being properly passed to the vector functions above.
	I would like to replace them with something a bit more... efficient :)

	I plan on eventually giving the GUI its own registry. Why not do it now?
	This way we can have much finer control over GUI elements.

	So:
	1. Create a registry for the GUI (DONE)
	2. Create entities for the EntityInspector and ComponentInspector
	3. On creation, emplace the relevant vectors
	4. See if it works :)

	Also, this file is getting BLOATED. Would be good to use a traditional .h/.cpp structure.
	Probably one header and a couple implementations.
	That way we can move the vector functions to Utility.h as well :)

	## Other things TODO:

	1. Implement Copy/Cut/Paste clipboard functions (with additional vectors)
	2. Implement drag'n'drop reordering and component shuffling/exchange

	*/

	void EntityInspector() {
		auto ctx = checkContext();
		auto io = ImGui::GetIO();
		static std::vector<listEntity*> entitiesSelected;
		static std::vector<listComponent*> componentsSelected;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

		//-- primary listing
		for (listEntity& e : entityList) {
			ImGuiTreeNodeFlags entityFlags = flags;
			bool entitySelected = find(entitiesSelected, &e);

			if (entitySelected)
				entityFlags |= ImGuiTreeNodeFlags_Selected;

			if (ImGui::TreeNodeEx(e.name.c_str(), entityFlags, e.name.c_str())) {
				if (e.componentList.empty()) {
					ImGui::Text("(empty)");
					ImGui::TreePop();
					continue;
				}

				for (listComponent& c : e.componentList) {
					ImGuiTreeNodeFlags componentFlags = flags;
					bool componentSelected = find(componentsSelected, &c);

					if (componentSelected)
						componentFlags |= ImGuiTreeNodeFlags_Selected;

					if (ImGui::TreeNodeEx(c.index.name.c_str(), componentFlags, c.index.name.c_str())) {
						c.index.f(c.meta);
						ImGui::TreePop();
					}

					if (ImGui::IsItemClicked(0) || (!componentSelected && ImGui::IsItemClicked(1))) {
						if (io.KeyCtrl) {
							addOrReplace(entitiesSelected, &e);
							addOrReplace(componentsSelected, &c);
						}
						else {
							entitiesSelected.clear();
							entitiesSelected.push_back(&e);
							componentsSelected.clear();
							componentsSelected.push_back(&c);
						}
					}
				}

				ImGui::TreePop();
			}

			if (ImGui::IsItemClicked(0) || (!entitySelected && ImGui::IsItemClicked(1))) {
				if (io.KeyCtrl) {
					addOrReplace(entitiesSelected, &e);

					//temp
					printf("logging selected list elements...\n");
					for (auto e : entitiesSelected) {
						printf("\"%s\": #%i\n", e->name.c_str(), e->entity);
					}
					for (auto c : componentsSelected) {
						printf("\"%s\": #%i\n", c->index.name.c_str(), c->index.id);
					}
				}
				else {
					entitiesSelected.clear();
					entitiesSelected.push_back(&e);

					//temp
					printf("logging selected list elements...\n");
					for (auto e : entitiesSelected) {
						printf("\"%s\": #%i\n", e->name.c_str(), e->entity);
					}
					for (auto c : componentsSelected) {
						printf("\"%s\": #%i\n", c->index.name.c_str(), c->index.id);
					}
				}
			}
		}

		//-- right click menu

		//note: probably not necessary to call this every time you click something
		auto closeContextMenu = [&]() -> void {
			entitiesSelected.clear();
			componentsSelected.clear();
		};

		if (ImGui::BeginPopupContextWindow()) {

			if (ImGui::MenuItem("Create Entity")) {
				//temp
				printf("logging selected list elements...\n");
				for (auto e : entitiesSelected) {
					printf("\"%s\": #%i\n", e->name.c_str(), e->entity);
				}
				for (auto c : componentsSelected) {
					printf("\"%s\": #%i\n", c->index.name.c_str(), c->index.id);
				}

				closeContextMenu();
			}

			if (entitiesSelected.size()) {
				std::string str = (entitiesSelected.size() == 1) ? "Entity" : "Entities";
				if (entitiesSelected.size() == 1 && ImGui::MenuItem(("Rename " + str).c_str())) {
					closeContextMenu();
				}
				if (ImGui::MenuItem(("Delete " + str).c_str())) {
					closeContextMenu();
				}
				if (ImGui::MenuItem(("Copy " + str).c_str())) {
					closeContextMenu();
				}
			}

			if (componentsSelected.size()) {
				std::string str = (componentsSelected.size() == 1) ? "Component" : "Components";
				if (ImGui::MenuItem("Open in Component Inspector")) {
					closeContextMenu();
				}
				if (ImGui::MenuItem(("Remove " + str).c_str())) {
					closeContextMenu();
				}
				if (ImGui::MenuItem(("Copy " + str).c_str())) {
					closeContextMenu();
				}
			}

			//TODO: clipboard

			ImGui::EndPopup();
		}
		else {
			closeContextMenu();
		}

	}
	void ComponentInspector() {
		using namespace ImGui;
		Text("WIP");
	}

	void stateRouter(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize) {
		using namespace ImGui;

		// ECS Tools
		if (gui.state[entityInspector]) {
			Begin("Entity Inspector");
			EntityInspector();
			End();
		}
		if (gui.state[componentInspector]) {
			Begin("Component Inspector");
			ComponentInspector();
			End();
		}
	}
	void mainMenu(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize) {
		using namespace ImGui;
		auto& io = ImGui::GetIO();
		if (BeginMainMenuBar()) {
			std::string label;
			if (BeginMenu("File")) {
				ImGui::EndMenu();
			}
			if (BeginMenu("Scenes")) {
				ImGui::EndMenu();
			}
			if (BeginMenu("Entities")) {
				label = gui.state[entityInspector] ? "Hide Entity Inspector" : "Show Entity Inspector";
				if (MenuItem(label.c_str()))
					setState(evw, gui, entityInspector, !gui.state[entityInspector]);
				ImGui::EndMenu();
			}
			if (BeginMenu("Components")) {
				label = gui.state[componentInspector] ? "Hide Component Inspector" : "Show Component Inspector";
				if (MenuItem(label.c_str()))
					setState(evw, gui, componentInspector, !gui.state[componentInspector]);
				ImGui::EndMenu();
			}
			if (BeginMenu("Window")) {
				ImGui::EndMenu();
			}
			EndMainMenuBar();
		}
	}
}
#endif