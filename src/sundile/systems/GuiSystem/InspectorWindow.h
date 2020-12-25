#ifndef GUI_INSPECTOR_H
#define GUI_INSPECTOR_H

#include "GuiTypes.h"
#include "GuiUtility.h"

namespace sundile::GuiSystem {
	/*

	\\todo

	0. Debug multi-select and implement entity renaming (will need to be saved when implementing ProjectSystem)
	1. Implement Copy/Cut/Paste clipboard functions
	2. Implement drag'n'drop reordering and component shuffling/exchange

	Would Be Nice:
	shift selection
	drag selection

	*/

	// [SECTION] - Forward declaration

	void EntityInspector(guiContainer& container);
	void ComponentInspector(guiContainer& container, std::string name);

	// [SECTION] - Editor windows

	void EntityInspector(guiContainer& container) {
		auto ctx = checkContext();
		auto io = ImGui::GetIO();
		auto clipboard = getClipboard<listEntity>();
		auto& selected = clipboard->selected;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

		//-- rendering
		for (listEntity& e : entityList) {
			ImGuiTreeNodeFlags entityFlags = flags;
			bool entitySelected = find(selected, &e);

			if (entitySelected)
				entityFlags |= ImGuiTreeNodeFlags_Selected;

			//-- render listEntities
			bool entityOpen = ImGui::TreeNodeEx(e.name.c_str(), entityFlags, e.name.c_str());
			//-- entity header clicked
			if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
				if (io.KeyCtrl) {
					addOrReplace(selected, &e);
				}
				else {
					selected.clear();
					selected.push_back(&e);
					container.state[e.name] = true;
				}
			}
			if (entityOpen) {
				ImGui::TreePop();
			}
		}

		//render ComponentInspectors
		for (auto i : container.state) {
			if (i.second) {
				ImGui::Begin((i.first + " - Component Inspector").c_str(), &i.second);
				ComponentInspector(container, i.first);
				ImGui::End();
			}
		}

		ClipboardContextMenu(clipboard, "Entity", "Entities");

	}
	void ComponentInspector(guiContainer& container, std::string name) {

		// \\ todo: if component inspector is open, create a new tab

		auto ctx = checkContext();
		auto io = ImGui::GetIO();
		auto clipboard = getClipboard<listComponent>();
		auto& selected = clipboard->selected;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		listEntity e;
		for (auto& i : entityList) {
			if (i.name == name) {
				e = i;
				break;
			}
		}
		if (e.componentList.empty()) {
			container.state[name] = false;
			printf("Unable to find entity with name: %s", name.c_str());
			return;
		}

		// Render listComponents
		for (listComponent& c : e.componentList) {
			ImGuiTreeNodeFlags componentFlags = flags;
			bool componentSelected = find(selected, &c);

			if (componentSelected)
				componentFlags |= ImGuiTreeNodeFlags_Selected;

			//-- component header clicked
			auto componentNodeOpen = ImGui::TreeNodeEx(c.index.name.c_str(), componentFlags, c.index.name.c_str());
			if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
				if (io.KeyCtrl) {
					addOrReplace(selected, &c);
				}
				else {
					selected.clear();
					selected.push_back(&c);
				}
			}
			if (componentNodeOpen) {
				//render the component's function
				c.index.f(c.meta);
				ImGui::TreePop();
			}
		}

		ClipboardContextMenu(clipboard, "Component", "Components");
	}
}

#endif