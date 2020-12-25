#ifndef GUI_FRONTEND_H
#define GUI_FRONTEND_H

#include "GuiTypes.h"
#include "GuiUtility.h"
#include "InspectorWindow.h"

namespace sundile::GuiSystem {

	// [SECTION] - Forward declaration
	void ComponentEditor(guiContainer& container);
	void stateRouter(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize);
	void mainMenu(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize);
	void initGuiFrontend();

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