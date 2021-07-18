#ifndef GUI_FRONTEND_H
#define GUI_FRONTEND_H

#include "GuiTypes.h"
#include "GuiUtility.h"

SYSTEM(GuiSystem)

	void mainMenu(guiContainer& gui) {
		using namespace ImGui;
		auto& io = ImGui::GetIO();
		if (BeginMainMenuBar()) {
			if (BeginMenu("Project")) {
				if (MenuItem("Save Project"))
					ProjectSystem::saveProject();

				ImGui::EndMenu();
			}

			std::string label;
			if (BeginMenu("Debug")) {
				label = gui.state["demo"] ? "Hide ImGUI Demo" : "Show ImGui Demo";
				if (MenuItem(label.c_str()))
					gui.state["demo"] = !gui.state["demo"];

				label = gui.state["metrics"] ? "Hide ImGUI Metrics" : "Show ImGui Metrics";
				if (MenuItem(label.c_str()))
					gui.state["metrics"] = !gui.state["metrics"];

				ImGui::EndMenu();
			}

			if (gui.state["demo"]) {
				ImGui::ShowDemoWindow(&gui.state["demo"]);
			}
			if (gui.state["metrics"]) {
				ImGui::ShowMetricsWindow(&gui.state["metrics"]);
			}

			if (ImGui::Button("Render View")) {
				gui.state["render view"] = true;
			}
			if (ImGui::Button("Node View")) {
				gui.state["node view"] = true;
			}

			EndMainMenuBar();
		}
	}
	void stateRouter(guiContainer& gui, guiContainer& parent = primaryGuiContainer) {
		for (auto pair : gui.styleVars) {
			ImGui::PushStyleVar(pair.first, pair.second);
		}
		ImGui::Begin(gui.name.c_str(), &(parent.state[gui.name]), gui.windowFlags);
		gui.renderFunc(gui);
		for (auto child : gui.children) {
			if (gui.state[child.name]) {
				stateRouter(child);
			}
		}
		ImGui::End();
		ImGui::PopStyleVar(gui.styleVars.size());
	}

END_SYSTEM

#endif