#ifndef GUI_DEBUG_VIEW
#define GUI_DEBUG_VIEW

SYSTEM(GuiSystem)
namespace DebugView {

	std::string name;
	char nameBuffer[256];

	void render(guiContainer& container) {
		auto windowCurspos = WindowSystem::currentWindow->input.cursorpos;
		auto renderCurspos = SceneSystem::currentScene->input.cursorpos;
		auto& sceneInput = SceneSystem::currentScene->input;
		ImGui::Text("Window cursorpos: (%f, %f)",windowCurspos.x, windowCurspos.y);

		ImGui::Text("RENDERER DEBUG INFO ===========");
		ImGui::Text("Renderer cursorpos: (%f, %f)", renderCurspos.x, renderCurspos.y);
		ImGui::Text("Renderer cursorpos_prev: (%f, %f)", sceneInput.cursorpos_prev.x, sceneInput.cursorpos_prev.y);
		ImGui::Text("Renderer cursorpos delta: (%f, %f)", renderCurspos.x - sceneInput.cursorpos_prev.x, renderCurspos.y - sceneInput.cursorpos_prev.y);
		ImGui::Text("mb_left: (%i, %i, %i)",
			InputSystem::isPressed(sceneInput, btn::mb_left),
			InputSystem::isHeld(sceneInput, btn::mb_left),
			InputSystem::isReleased(sceneInput, btn::mb_left));
		ImGui::Text("mb_right: (%i, %i, %i)",
			InputSystem::isPressed(sceneInput, btn::mb_right),
			InputSystem::isHeld(sceneInput, btn::mb_right),
			InputSystem::isReleased(sceneInput, btn::mb_right));

		ImGui::Text("CURRENT SCENE DEBUG INFO =============");
		ImGui::Text("registry size:, %i", SceneSystem::currentScene->registry->size());
		if (ImGui::Button("updateEntities(currentScene)")) {
			SceneSystem::updateEntities(SceneSystem::currentScene);
		}

	}

	void destroy(TerminateEvent<SmartEVW>& ev) {

	}
	void init() {
		guiContainer debugView("debug view", DebugView::render);
		primaryGuiContainer.state["debug view"] = true;
		primaryGuiContainer.children.push_back(debugView);
		EventSystem::currentEVW->dispatcher.sink<TerminateEvent<SmartEVW>>().connect<&destroy>();
	}
}

END_SYSTEM

#endif