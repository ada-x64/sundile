#ifndef GUI_UTILITY_H
#define GUI_UTILITY_H

namespace sundile::GuiSystem {
	//-- HELPERS
	auto checkContext() {
		auto ctx = ImGui::GetCurrentContext();
		if (!ctx) { ctx = ImGui::CreateContext(); ImGui::SetCurrentContext(ctx); }
		return ctx;
	}
	void setState(const SmartEVW& evw, guiContainer& gui, GuiStateKey key, bool value) {
		gui.state[key] = value;
		GuiEvent ev(getRegistryByID(evw,currentSim), { key, value });
		evw->dispatcher.trigger<GuiEvent>(ev);
	}
	void terminate(const terminateEvent& ev) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void stateSetter(const SmartEVW& evw, guiContainer& gui, ImVec2 windowSize) {
		using namespace ImGui;
		auto& io = ImGui::GetIO();
		if (IsWindowFocused(ImGuiFocusedFlags_AnyWindow)) {
			if (!gui.state[focusAny]) {
				setState(evw, gui, focusAny, true);
			}
		}
		else if (gui.state[focusAny]) {
			setState(evw, gui, focusAny, false);
		}
	}
}


#endif