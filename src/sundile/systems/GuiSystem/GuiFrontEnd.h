#ifndef GUI_FRONTEND_H
#define GUI_FRONTEND_H

#include "GuiTypes.h"
#include "GuiUtility.h"
#include "GuiInspectorWindow.h"

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
		guiRegistry.view<guiContainer>().each([&](entt::entity e, guiContainer& container) {
			if (e == primaryGuiEntity) {
				container.size = windowSize;
			}
			for (auto i : gui.state) {
				if (i.second && container.name == i.first) {
					SetNextWindowSize(container.size, ImGuiCond_Appearing);
					Begin(container.name.c_str(),&(gui.state[container.name]),container.windowFlags);
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
			if (BeginMenu("Window")) {
				if (MenuItem("Show ImGUI Demo")) showDemo = !showDemo;
				if (MenuItem("Show ImGUI Metrics")) showMetrics = !showMetrics;

				label = gui.state["Entity Inspector"] ? "Hide Inspector" : "Show Inspector";
				if (MenuItem(label.c_str()))
					setState(evw, gui, "Inspector", !gui.state["Inspector"]);

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
		auto inspector = guiRegistry.create();
		auto inspectorContainer = guiRegistry.emplace<guiContainer>(inspector, "Inspector", Inspector, ImVec2(0.f, 300.f), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoMove);
		getPrimaryContainer()->state["Inspector"] = true;
		initInspector(inspectorContainer);
	}
}
#endif