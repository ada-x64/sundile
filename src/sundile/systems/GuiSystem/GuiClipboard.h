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
	void ClipboardContextMenu(guiClipboard<T>* clipboard, guiTreeContainer<T>& treeContainer, std::string singular = "", std::string plural = "", std::string* itemName = nullptr) {
		auto& selected = clipboard->selected;
		auto& list = clipboard->list;
		auto& name = (selected.size() == 1) ? singular : plural;
		auto& rename = clipboard->state["rename"];
		auto& toBeRenamed = clipboard->toBeRenamed;
		auto* namebuff = clipboard->namebuff;
		auto& tree = treeContainer.tree;

		// \\todo: implement keyboard shortcuts
		// \\todo: finish implementing copy/cut/paste functionality

		// Right click menu
		if (ImGui::BeginPopupContextWindow()) {

			if (ImGui::MenuItem("Create New Folder", "CTRL+G")) {
				T* nullObject = new T();
				listNodeRef<T> folder = std::make_shared<listNode<T>>(*nullObject);
				folder->name = "New Folder";
				folder->state["folder"] = true;

				//replace this with recursive search
				bool found = false;
				for (auto i = 0; i < tree.size(); ++i) {
					for (auto j = 0; j < selected.size(); ++j) {
						if (tree.at(i)->content.get() == selected.at(j)) {
							tree.insert(tree.begin() + i, folder);
							found = true;
							break;
						}
					}
					if (found) break;
				}
				if (!found) {
					tree.push_back(folder);
				}
				selected.clear();
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
				if (ImGui::MenuItem("Open in Component Editor"), "ALT+TAB") {
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