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

	// [SECTION] - Structs & namespace globals

	//static std::vector<sceneTab> sceneTabs;
	static std::vector<entityTab> entityTabs;
	static std::vector<componentTab> componentTabs;
	static std::vector<dataTab> dataTabs;

	// [SECTION] - Editor windows
	
	static const nodeEventCallback<listComponent> componentTab_LeftClick = [&](listNodeRef<listComponent> p_node, guiTreeContainer<listComponent>& tree) {
		auto io = ImGui::GetIO();
		auto clipboard = getClipboard<listComponent>();
		auto& selected = clipboard->selected;
		auto p_component = p_node->content.get();

		if (io.KeyCtrl) {
			p_node->state["selected"] = true;
			addOrReplace(selected, p_component);
		}
		else {
			ClearGuiTreeSelectionState(tree);
			selected.clear();
			selected.push_back(p_component);
			p_node->state["selected"] = true;

			bool found = false;
			for (auto i : dataTabs) {
				if (i.name == p_component->name) {
					found = true; break;
				}
			}
			if (!found) {
				dataTab tab(p_component->name, p_component->index.f, p_component->meta);
				dataTabs.emplace_back(tab);
			}
		}
	};
	componentTab createComponentTab(std::string name, guiComponentList& components) {
		//create component tab
		componentTab tab(name, [](guiContainer&, guiTreeContainer<listComponent> tree) {
			RenderGuiTree(tree);
			ClipboardContextMenu(getClipboard<listComponent>(), tree, "Component", "Components");
			}, &components);
		tab.treeContainer.callbacks[leftClick] = componentTab_LeftClick;
		return componentTabs.emplace_back<componentTab>(std::move(tab));
	};
	
	static const nodeEventCallback<listEntity> entityTab_LeftClick = [&](listNodeRef<listEntity> p_node, guiTreeContainer<listEntity>& tree) {
		auto io = ImGui::GetIO();
		auto clipboard = getClipboard<listEntity>();
		auto& selected = clipboard->selected;
		auto p_entity = p_node->content.get();

		if (io.KeyCtrl) {
			p_node->state["selected"] = true;
			addOrReplace(selected, p_entity);
		}
		else {
			ClearGuiTreeSelectionState(tree);
			selected.clear();
			selected.push_back(p_entity);
			p_node->state["selected"] = true;

			bool found = false;
			for (auto i : componentTabs) {
				if (i.name == p_entity->name) {
					found = true; break;
				}
			}
			if (!found) {
				createComponentTab(p_entity->name, p_entity->componentList);
			}
		}
	};
	entityTab& createEntityTab(std::string name, guiEntityList& entities) {
		entityTab tab(name, [](guiContainer&, guiTreeContainer<listEntity>& tree) {
			RenderGuiTree(tree);
			ClipboardContextMenu(getClipboard<listEntity>(), tree, "Entity", "Entities");
			}, &entities);
		tab.container.state["open"] = true;
		tab.treeContainer.callbacks[leftClick] = entityTab_LeftClick;

		return entityTabs.emplace_back<entityTab>(std::move(tab));
	}

	// To be called on GuiInit, which should be called when GUI is loaded from ProjectSystem
	void initInspector(guiContainer& container) {
		//Initialize Tabs (should be scene instead of entities, but this is fine for now)
		createEntityTab("[scene name]", entityList);
	}
	void Inspector(guiContainer& container) {
		auto ctx = checkContext();
		auto& io = ImGui::GetIO();

		auto& primary = guiRegistry.get<guiContainer>(primaryGuiEntity);
		auto maxSize = primary.size;

		float numChildren = 4.f;
		auto width = ImGui::GetWindowWidth() / numChildren;

		//need some storage for selected entities, components, &c.
		//figure out how to use omittable variadic template parameters like entt::emplace
		ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton;

		ImGui::SetWindowSize(ImVec2(0.f, 300.f));
		ImGui::SetWindowPos(ImVec2(0.f, maxSize.y - ImGui::GetWindowHeight()));

		ImGui::BeginChild("Scene Selector", ImVec2(width, 0), true);
		ImGui::EndChild();

		ImGui::SameLine();
		if (ImGui::BeginChild("Entity Selector", ImVec2(width, 0), true)) {
			if (ImGui::BeginTabBar("Entity Selector Tabs", tabBarFlags)) {
				RenderTabs<entityTab>(entityTabs);
				ImGui::EndTabBar();
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();
		if (ImGui::BeginChild("Component Selector", ImVec2(width, 0), true)) {
			if (ImGui::BeginTabBar("Component Selector Tabs", tabBarFlags)) {
				RenderTabs<componentTab>(componentTabs);
				ImGui::EndTabBar();
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();
		if (ImGui::BeginChild("Data Editor", ImVec2(width, 0), true)) {
			if (ImGui::BeginTabBar("Data Editor Tabs", tabBarFlags)) {
				RenderTabs<dataTab>(dataTabs);
				ImGui::EndTabBar();
			}
		}
		ImGui::EndChild();

	}
}

#endif