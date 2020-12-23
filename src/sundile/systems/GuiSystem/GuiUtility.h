#ifndef GUI_UTILITY_H
#define GUI_UTILITY_H

namespace sundile::GuiSystem {
	//-- HELPERS
	auto checkContext() {
		auto ctx = ImGui::GetCurrentContext();
		if (!ctx) { ctx = ImGui::CreateContext(); ImGui::SetCurrentContext(ctx); }
		return ctx;
	}
	void setState(const SmartEVW& evw, guiContainer& gui, const char* key, bool value) {
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
			if (!gui.state["focus any"]) {
				setState(evw, gui, "focus any", true);
			}
		}
		else if (gui.state["focus any"]) {
			setState(evw, gui, "focus any", false);
		}
	}

	guiContainer* getPrimaryContainer() {
		return &guiRegistry.get<guiContainer>(primaryGuiEntity);
	}
	
	template<typename T>
	guiClipboard<T>* getClipboard() {
		auto clipboard = clipboardList[typeid(T).name()];
		if (clipboard == nullptr) {
			clipboard = (void*)(new guiClipboard<T>);
			clipboardList[typeid(T).name()] = clipboard;
		}
		return (guiClipboard<T>*)(clipboard);
	}

	template<typename T>
	void ClipboardContextMenu(guiClipboard<T>* clipboard, std::string singular = "", std::string plural = "", std::string* itemName = nullptr) {
		auto& selected = clipboard->selected;
		auto& list = clipboard->list;
		auto& name = (selected.size() == 1) ? singular : plural;
		auto& rename = clipboard->state["rename"];
		auto& toBeRenamed = clipboard->toBeRenamed;
		auto* namebuff = clipboard->namebuff;

		// \\todo: implement keyboard shortcuts
		// \\todo: finish implementing copy/cut/paste functionality

		// Right click menu
		if (ImGui::BeginPopupContextWindow()) {

			if (ImGui::MenuItem(("Create " + singular).c_str(), "CTRL+N")) {
				selected.clear();
			}

			if (list.size()) {
				if (ImGui::MenuItem(("Paste " + name).c_str(), "CTRL+V")) {
				}
			}

			if (selected.size()) {
				if (selected.size() == 1) {
					if (ImGui::MenuItem(("Rename " + name).c_str(), "F2")) {
						rename = true;
					}
				}

				if (ImGui::MenuItem(("Cut " + name).c_str(), "CTRL+X")) {
					for (auto i = 0; i < selected.size(); ++i) {
						list.emplace_back(*selected[i]);
					}
					selected.clear();
				}
				if (ImGui::MenuItem(("Copy " + name).c_str(), "CTRL+C")) {
					for (auto i = 0; i < selected.size(); ++i) {
						list.emplace_back(*selected[i]);
					}
					selected.clear();
				}
				if (ImGui::MenuItem(("Delete " + name).c_str(), "DEL")) {
					selected.clear();
				}
			}

			if (typeid(T) == typeid(listComponent)) {
				if (ImGui::MenuItem("Open in Component Editor"),"ALT+TAB") {
					selected.clear();
				}
			}

			ImGui::EndPopup(); //end rightclick menu
		}

		//rename
		if (rename && itemName != nullptr) {
			ImGui::OpenPopup("rename");
			toBeRenamed = selected[0];
			strcpy(namebuff, itemName->c_str());
		}
		if (ImGui::BeginPopup("rename")) {
			rename = false;
			ImGui::Text("Edit name:");
			bool enterPressed = ImGui::InputText("##edit", namebuff, IM_ARRAYSIZE(namebuff), ImGuiInputTextFlags_EnterReturnsTrue);
			bool confirmPressed = ImGui::Button("Confirm");
			if (enterPressed || confirmPressed) {
				itemName->assign(namebuff);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}


#endif