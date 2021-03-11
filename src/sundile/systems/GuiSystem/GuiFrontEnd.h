#ifndef GUI_FRONTEND_H
#define GUI_FRONTEND_H

#include "GuiTypes.h"
#include "GuiUtility.h"
#include "GuiInspectorWindow.h"

BEGIN_SYSTEM(GuiSystem)
	// [SECTION] - Front-end (excluding ECS inspector tools)

	//-- Editor Toolbar
	void editorToolbar(guiContainer& container) {
		if (ImGui::Button("select")) {
			container.state["select"] = true;
		}
	}
	//-- Editor Renderer
	void editorFrame(guiContainer& container) {
		if (WindowSystem::currentWindow != nullptr) {
			auto gui = getPrimaryContainer();
			if (gui->state["disable input"] == ImGui::IsWindowFocused()) {
				gui->state["disable input"] = !gui->state["disable input"];
				container.windowFlags ^= ImGuiWindowFlags_NoMove;
			}

			GLfloat viewport[4];
			glGetFloatv(GL_VIEWPORT, viewport);
			auto x = viewport[0];
			auto y = viewport[1];
			auto w = viewport[2];
			auto h = viewport[3];

			auto& window = WindowSystem::currentWindow;
			auto& renderer = RenderSystem::currentRenderer;

			//modify size to aspect ratio
			auto ratio = (renderer->size.x / renderer->size.y);
			auto padding = ImGui::GetTextLineHeightWithSpacing();
			auto windowSize = ImGui::GetWindowSize();
			windowSize = {windowSize.x , (windowSize.x / ratio) - padding};
			ImGui::SetWindowSize({ windowSize.x, windowSize.y + padding});

			Vec2 pos = ImGui::GetWindowPos();
			pos.y = window->HEIGHT - pos.y - padding - windowSize.y;
			glViewport(pos.x, pos.y, windowSize.x, windowSize.y);
		}
	}
	//-- Main Menu
	void mainMenu(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize) {
		using namespace ImGui;
		auto& io = ImGui::GetIO();
		if (BeginMainMenuBar()) {
			std::string label;
			if (BeginMenu("File")) {
				ImGui::EndMenu();
			}
			if (BeginMenu("Scene")) {
				if (MenuItem("Toggle Wireframe View")) gui.state["wireframe"] = !gui.state["wireframe"];
				ImGui::EndMenu();
			}
			if (BeginMenu("Window")) {
				label = gui.state["Entity Inspector"] ? "Hide Entity Inspector" : "Show Entity Inspector";
				if (MenuItem(label.c_str()))
					setState(evw, gui, "Inspector", !gui.state["Inspector"]);

				ImGui::EndMenu();
			}
			if (BeginMenu("Debug")) {
				label = gui.state["demo"] ? "Hide ImGUI Demo" : "Show ImGui Demo";
				if (MenuItem(label.c_str())) gui.state["demo"] = !gui.state["demo"];
				label = gui.state["metrics"] ? "Hide ImGUI Metrics" : "Show ImGui Metrics";
				if (MenuItem(label.c_str())) gui.state["metrics"] = !gui.state["metrics"];
				ImGui::EndMenu();
			}

			if (gui.state["demo"]) {
				ImGui::ShowDemoWindow(&gui.state["demo"]);
			}
			if (gui.state["metrics"]) {
				ImGui::ShowMetricsWindow(&gui.state["metrics"]);
			}
			EndMainMenuBar();
		}
	}

	// [SECTION] - Back-End
	//-- State Router
	void stateRouter(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize) {
		// ECS Tools
		guiRegistry.view<guiContainer>().each([&](entt::entity e, guiContainer& container) {
			if (e == primaryGuiEntity) {
				container.size = windowSize;
			}
			for (auto i : gui.state) {
				if (i.second && container.name == i.first) {
					for (auto pair : container.styleVars) {
						ImGui::PushStyleVar(pair.first, pair.second);
					}
					ImGui::Begin(container.name.c_str(), &(gui.state[container.name]), container.windowFlags);
					container.renderFunc(container);
					ImGui::End();
					ImGui::PopStyleVar(container.styleVars.size());
				}
			}
			});

		GuiEvent ev(getRegistryByID(evw, currentScene), gui.state);
		evw->dispatcher.trigger<GuiEvent>(ev);
	}
	//-- Init Frontend
	void initGuiFrontend(SmartEVW& evw) {

		// parent window
		getPrimaryContainer()->state["disable input"] = false;

		// inspectors
		auto inspector = guiRegistry.create();
		auto inspectorContainer = guiRegistry.emplace<guiContainer>(inspector, "Inspector", Inspector, ImVec2(0.f, 300.f), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoMove);
		getPrimaryContainer()->state["Inspector"] = true;
		initInspector(inspectorContainer);

		// main editor
		GLfloat viewport[4];
		glGetFloatv(GL_VIEWPORT, viewport);
		auto editor = guiRegistry.create();
		auto editorContainer = guiRegistry.emplace<guiContainer>(editor, "Editor", editorFrame, ImVec2( 0,0 ),
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove
			);
		getPrimaryContainer()->state["Editor"] = true;
		editorContainer.state["select"] = true;

		//editor toolbar
		auto toolbar = guiRegistry.create();
		auto toolbarContainer = guiRegistry.emplace<guiContainer>(toolbar, "Editor Toolbar", editorToolbar, ImVec2(0, 0));
		getPrimaryContainer()->state["Editor Toolbar"] = true;

		////\TODO
		//evw->dispatcher.sink<preStepEvent>().connect< [&editorContainer](preStepEvent&) {
		//	SceneSystem::parseGuiState(editorContainer.state);
		//}>();
	}

END_SYSTEM

#include "GuiClipboard.h"
#endif