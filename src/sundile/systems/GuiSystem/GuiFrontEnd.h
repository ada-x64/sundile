#ifndef GUI_FRONTEND_H
#define GUI_FRONTEND_H

#include "GuiTypes.h"
#include "GuiUtility.h"
#include "GuiInspectorWindow.h"

namespace sundile::GuiSystem {

	// [SECTION] - Forward declaration
	void stateRouter(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize);
	void mainMenu(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize);
	void initGuiFrontend();
	
	// [SECTION] - Back-End and Main Menu

	void editorFrame(guiContainer& container) {

		if (WindowSystem::currentWindow != nullptr) {
			GLfloat viewport[4];
			glGetFloatv(GL_VIEWPORT, viewport);
			auto x = viewport[0];
			auto y = viewport[1];
			auto w = viewport[2];
			auto h = viewport[3];

			auto& window = WindowSystem::currentWindow;
			auto& renderer = RenderSystem::currentRenderer;

			//ImGui::SetWindowSize("Editor", {w,h});
			ImVec2 size = ImGui::GetWindowSize();
			ImVec2 pos = ImGui::GetWindowPos();
			//modify size to aspect ratio
			size = { size.x, size.x * renderer->size.y / renderer->size.x };

			glViewport(pos.x, window->HEIGHT - pos.y - size.y, size.x, size.y);
			ImGui::SetWindowSize(size);
		}
	}

	void stateRouter(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize) {
		using namespace ImGui;
		// ECS Tools
		guiRegistry.view<guiContainer>().each([&](entt::entity e, guiContainer& container) {
			if (e == primaryGuiEntity) {
				container.size = windowSize;
			}
			for (auto i : gui.state) {
				if (i.second && container.name == i.first) {
					//SetNextWindowSize(container.size, ImGuiCond_Appearing);
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

		//main editor
		GLfloat viewport[4];
		glGetFloatv(GL_VIEWPORT, viewport);
		auto editor = guiRegistry.create();
		auto editorContainer = guiRegistry.emplace<guiContainer>(editor, "Editor", editorFrame, ImVec2(viewport[2],viewport[3]), ImGuiWindowFlags_NoBackground);
		getPrimaryContainer()->state["Editor"] = true;
	}
}
#endif