#ifndef GUI_FRONTEND_H
#define GUI_FRONTEND_H

#include "GuiTypes.h"
#include "GuiUtility.h"

namespace sundile::GuiSystem {

	// [SECTION] - Forward declaration
	void EntityInspector(guiContainer& container);
	void ComponentInspector(guiContainer& container, std::string name);
	void ComponentEditor(guiContainer& container);
	void stateRouter(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize);
	void mainMenu(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize);
	void initGuiFrontend();

	/*

	\\todo

	0. Debug multi-select and implement entity renaming (will need to be saved when implementing ProjectSystem)
	1. Implement Copy/Cut/Paste clipboard functions
	2. Implement drag'n'drop reordering and component shuffling/exchange
	
	Would Be Nice:
	shift selection
	drag selection

	*/

	// [SECTION] - Editor windows

	void EntityInspector(guiContainer& container) {
		auto ctx = checkContext();
		auto io = ImGui::GetIO();
		auto clipboard = getClipboard<listEntity>();
		auto& selected = clipboard->selected;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

		//-- rendering
		for (listEntity& e : entityList) {
			ImGuiTreeNodeFlags entityFlags = flags;
			bool entitySelected = find(selected, &e);

			if (entitySelected)
				entityFlags |= ImGuiTreeNodeFlags_Selected;

			//-- render listEntities
			bool entityOpen = ImGui::TreeNodeEx(e.name.c_str(), entityFlags, e.name.c_str());
			//-- entity header clicked
			if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) ) {
				if (io.KeyCtrl) {
					addOrReplace(selected, &e);
				}
				else {
					selected.clear();
					selected.push_back(&e);
					container.state[e.name] = true;
				}
			}
			if (entityOpen) {
				ImGui::TreePop();
			}
		}

		//render ComponentInspectors
		for (auto i : container.state) {
			if (i.second) {
				ImGui::Begin((i.first+" - Component Inspector").c_str(), &i.second);
				ComponentInspector(container, i.first);
				ImGui::End();
			}
		}

		ClipboardContextMenu(clipboard, "Entity", "Entities");

	}
	void ComponentInspector(guiContainer& container, std::string name) {

		// \\ todo: if component inspector is open, create a new tab

		auto ctx = checkContext();
		auto io = ImGui::GetIO();
		auto clipboard = getClipboard<listComponent>();
		auto& selected = clipboard->selected;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		listEntity e;
		for (auto& i : entityList) {
			if (i.name == name) {
				e = i;
				break;
			}
		}
		if (e.componentList.empty()) {
			container.state[name] = false;
			printf("Unable to find entity with name: %s", name.c_str());
			return;
		}

		// Render listComponents
		for (listComponent& c : e.componentList) {
			ImGuiTreeNodeFlags componentFlags = flags;
			bool componentSelected = find(selected, &c);

			if (componentSelected)
				componentFlags |= ImGuiTreeNodeFlags_Selected;

			//-- component header clicked
			auto componentNodeOpen = ImGui::TreeNodeEx(c.index.name.c_str(), componentFlags, c.index.name.c_str());
			if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
				if (io.KeyCtrl) {
					addOrReplace(selected, &c);
				}
				else {
					selected.clear();
					selected.push_back(&c);
				}
			}
			if (componentNodeOpen) {
				//render the component's function
				c.index.f(c.meta);
				ImGui::TreePop();
			}
		}

		ClipboardContextMenu(clipboard, "Component", "Components");
	}
	void ComponentEditor(guiContainer& container) {
		using namespace ImGui;
		Text("WIP");
	}
	
	// [SECTION] - Back-End and Main Menu

	void stateRouter(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize) {
		using namespace ImGui;
		// ECS Tools
		guiRegistry.view<guiContainer>().each([&](guiContainer& container) {
			for (auto i : gui.state) {
				if (i.second && container.name == i.first) {
					Begin(container.name.c_str(),&(gui.state[container.name]));
					container.renderFunc(container);
					End();
				}
			}
			});
	}
	void mainMenu(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize) {
		using namespace ImGui;
		auto& io = ImGui::GetIO();
		static bool showDemo, showMetrics;
		if (BeginMainMenuBar()) {
			std::string label;
			if (BeginMenu("File")) {
				ImGui::EndMenu();
			}
			if (BeginMenu("Scenes")) {
				ImGui::EndMenu();
			}
			if (BeginMenu("Entities")) {
				label = gui.state["Entity Inspector"] ? "Hide Entity Inspector" : "Show Entity Inspector";
				if (MenuItem(label.c_str()))
					setState(evw, gui, "Entity Inspector", !gui.state["Entity Inspector"]);
				ImGui::EndMenu();
			}
			if (BeginMenu("Components")) {
				label = gui.state["Component Editor"] ? "Hide Component Editor" : "Show Component Editor";
				if (MenuItem(label.c_str()))
					setState(evw, gui, "Component Editor", !gui.state["Component Editor"]);
				ImGui::EndMenu();
			}
			if (BeginMenu("Window")) {
				if (MenuItem("Show ImGUI Demo")) showDemo = !showDemo;
				if (MenuItem("Show ImGUI Metrics")) showMetrics = !showMetrics;
				ImGui::EndMenu();
			}
			if (showDemo) {
				ImGui::ShowDemoWindow(&showDemo);
			}
			if (showMetrics) {
				ImGui::ShowMetricsWindow(&showMetrics);
			}
			EndMainMenuBar();
		}
	}
	void initGuiFrontend() {
		// inspectors
		auto entityInspector = guiRegistry.create();
		guiRegistry.emplace<guiContainer>(entityInspector, "Entity Inspector", EntityInspector);
		auto componentEditor = guiRegistry.create();
		guiRegistry.emplace<guiContainer>(componentEditor, "Component Editor", ComponentEditor);

	}
}
#endif