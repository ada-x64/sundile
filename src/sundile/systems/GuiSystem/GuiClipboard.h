#ifndef GUI_CLIPBOARD
#define GUI_CLIPBOARD

namespace sundile::GuiSystem {
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
	void ClipboardContextMenu(guiClipboard<T>* clipboard, listNodeRef<T> root, std::string singular = "", std::string plural = "", std::string* itemName = nullptr) {
		auto& selected = clipboard->selected;
		auto& list = clipboard->list;
		auto& name = (selected.size() == 1) ? singular : plural;
		auto& rename = clipboard->state["rename"];
		auto& toBeRenamed = clipboard->toBeRenamed;
		auto* namebuff = clipboard->namebuff;
		auto& tree = root->children;
		static int newFolderCount = 0;

		// \\todo: implement keyboard shortcuts
		// \\todo: finish implementing copy/cut/paste functionality

		// Right click menu
		if (ImGui::BeginPopupContextWindow()) {

			if (ImGui::MenuItem("Create New Folder", "CTRL+G")) {
				T* nullObject = new T();
				listNodeRef<T> folder = std::make_shared<listNode<T>>(*nullObject);
				folder->name = "New Folder " + std::to_string(newFolderCount);
				folder->state["folder"] = true;
				clipboard->state["rename"] = true;
				clipboard->selected.push_back(folder);
				itemName = &(folder->name);

				//replace this with recursive search
				bool found = false;
				for (auto i = tree.begin(); i != tree.end(); ++i) {
					for (auto j = selected.begin(); j != selected.end(); ++j) {
						if (*i == *j) {
							tree.insert(i, folder);
							found = true;
							break;
						}
					}
					if (found) break;
				}
				if (!found) {
					tree.push_back(folder);
				}
			}

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
						list.emplace_back(selected[i]);
					}
					selected.clear();
				}
				if (ImGui::MenuItem(("Copy " + name).c_str(), "CTRL+C")) {
					for (auto i = 0; i < selected.size(); ++i) {
						list.emplace_back(selected[i]);
					}
					selected.clear();
				}
				if (ImGui::MenuItem(("Delete " + name).c_str(), "DEL")) {
					selected.clear();
				}
			}

			if (typeid(T) == typeid(listComponent)) {
				if (ImGui::MenuItem("Open in Component Editor"), "ALT+TAB") {
					selected.clear();
				}
			}

			ImGui::EndPopup(); //end rightclick menu
		}

		//rename
		if (rename && selected.size()) {
			ImGui::OpenPopup("rename");
			toBeRenamed = selected[0];
			strcpy(namebuff, selected[0]->name.c_str());
		}
		if (ImGui::BeginPopup("rename")) {
			rename = false;
			ImGui::Text("Edit name:");
			bool enterPressed = ImGui::InputText("##edit", namebuff, IM_ARRAYSIZE(namebuff), ImGuiInputTextFlags_EnterReturnsTrue);
			bool confirmPressed = ImGui::Button("Confirm");
			if ((enterPressed || confirmPressed) && selected.size()) {
				selected[0]->name.assign(namebuff);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}

#endif