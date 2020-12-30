#ifndef GUI_INSPECTOR_H
#define GUI_INSPECTOR_H

#include "GuiTypes.h"
#include "GuiUtility.h"

namespace sundile::GuiSystem {
	/*

	\\todo

	0. Debug multi-select and implement p_entity renaming (will need to be saved when implementing ProjectSystem)
	1. Implement Copy/Cut/Paste clipboard functions
	2. Implement drag'n'drop reordering and component shuffling/exchange

	Would Be Nice:
	shift selection
	drag selection

	*/

	// [SECTION] - Forward declaration
	void Inspector(guiContainer&);
	void EntityInspector(guiContainer&, guiTree<listEntity>);
	void ComponentInspector(guiContainer&, guiTree<listComponent>);

	// [SECTION] - Structs & namespace globals

	//static std::vector<sceneTab> sceneTabs;
	static std::vector<entityTab> entityTabs;
	static std::vector<componentTab> componentTabs;
	static std::vector<dataTab> dataTabs;

	static std::vector<int> toBeErased;

	static ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_None;
	static ImGuiTreeNodeFlags selectedFlags = ImGuiTreeNodeFlags_Selected;
	static ImGuiTreeNodeFlags leafFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	template <typename T>
	using nodeClickedFunc = std::function<void(listNode<T>*)>;

	// [SECTION] - Helper functions
	template<typename T>
	void RenderTabs(std::vector<T>& tabs) {
		int pos = 0;
		for (auto it = tabs.begin(); it != tabs.end(); ++it) {
			auto& tab = *it;
			if (tab.container.state["open"]) {
				tab.render();
			}
			else {
				toBeErased.push_back(pos);
			}
			++pos;

			if (tabs.empty() || it == tabs.end()) break;
		}
		for (auto i : toBeErased) {
			tabs.erase(tabs.begin() + i);
		}
		toBeErased.clear();
	}

	template<typename T>
	void RenderListNode(listNode<T>* p_node, nodeClickedFunc<T> leftClick, nodeClickedFunc<T> rightClick) {
		auto selectedStateFlags = (p_node->state["selected"] ? selectedFlags : 0);
		if (p_node->children.empty()) {
			ImGui::TreeNodeEx(p_node->name.c_str(), leafFlags | selectedStateFlags);
			if (ImGui::IsItemClicked(0)) {
				leftClick(p_node);
			}
			if (ImGui::IsItemClicked(1)) {
				rightClick(p_node);
			}
		}
		else {
			for (auto p_child : p_node->children) {
				if (ImGui::TreeNodeEx(p_node->name.c_str(), nodeFlags | selectedStateFlags)) {
					if (ImGui::IsItemClicked(0)) {
						leftClick(p_node);
					}
					if (ImGui::IsItemClicked(1)) {
						rightClick(p_node);
					}
					RenderListNode(p_child, leftClick, rightClick);
					ImGui::TreePop();
				}
			}
		}
	}

	template<typename T>
	void RenderGuiTree(guiTree<T> tree, nodeClickedFunc<T> leftClick, nodeClickedFunc<T> rightClick) {
		for (auto p_node : tree) {
			RenderListNode<T>(p_node, leftClick, rightClick);
		}
	}

	template<typename T>
	void ClearListNodeSelectionState(listNode<T>* p_node) {
		p_node->state["selected"] = false;
		for (auto p_child : p_node->children) {
			ClearListNodeSelectionState<T>(p_child);
		}
	}

	template <typename T>
	void ClearGuiTreeSelectionState(guiTree<T> tree) {
		for (auto p_node : tree) {
			ClearListNodeSelectionState(p_node);
		}
	}

	// [SECTION] - Editor windows
	void Inspector(guiContainer& container) {
		auto ctx = checkContext();
		auto& io = ImGui::GetIO();

		auto& primary = guiRegistry.get<guiContainer>(primaryGuiEntity);
		auto maxSize = primary.size;

		float numChildren = 4.f;
		auto width = ImGui::GetWindowWidth() / numChildren;

		//\todo: replace this;
		static bool initialized = false;
		if (!initialized) {
			entityTab tab("foo", EntityInspector, &entityList);
			tab.container.state["open"] = true;
			entityTabs.emplace_back<entityTab>(std::move(tab));
			initialized = true;
		}

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
	/*
	\TODO
	* convert to folder structure with tree as basis.
		will need to keep track of this in some sort of struct.
	* List items should be selectable, or bullets? Not quite sure atm
	*/
	void EntityInspector(guiContainer& container, guiTree<listEntity> tree) {
		auto ctx = checkContext();
		auto io = ImGui::GetIO();
		auto clipboard = getClipboard<listEntity>();
		auto& selected = clipboard->selected;
		ImGuiTreeNodeFlags flags = nodeFlags;

		//-- rendering
		// \todo: possibly replace clipboard->selected?
		nodeClickedFunc<listEntity> clicked = [&](listNode<listEntity>* p_node) {
			//\todo: corrupt pointers :(
			//maybe lookup by name?
			//or convert to shared pointers - would possibly require revamping global lists
			auto p_entity = p_node->content;

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
					componentTab tab(p_entity->name, ComponentInspector, &(p_entity->componentList));
					componentTabs.emplace_back<componentTab>(std::move(tab));
				}
			}

		};
		RenderGuiTree(tree, clicked, clicked);

		/**
		for (listEntity& e : entityList) {
			ImGuiTreeNodeFlags entityFlags = flags;
			bool entitySelected = find(selected, &e);

			if (entitySelected)
				entityFlags |= ImGuiTreeNodeFlags_Selected;

			//-- render listEntities
			bool entityOpen = ImGui::TreeNodeEx(e.name.c_str(), entityFlags, e.name.c_str());
			//-- p_entity header clicked
			if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
				if (io.KeyCtrl) {
					addOrReplace(selected, &e);
				}
				else {
					selected.clear();
					selected.push_back(&e);
					bool found = false;
					for (auto i : componentTabs) {
						if (i.name == e.name) {
							found = true; break;
						}
					}
					if (!found) {
						componentTab tab(e.name, ComponentInspector, e.componentList);
						componentTabs.emplace_back<componentTab>(std::move(tab));
					}
				}
			}
		}
		/**/

		ClipboardContextMenu(clipboard, "Entity", "Entities");

	}
	void ComponentInspector(guiContainer& container, guiTree<listComponent> tree) {
		auto ctx = checkContext();
		auto io = ImGui::GetIO();
		auto clipboard = getClipboard<listComponent>();
		auto& selected = clipboard->selected;
		ImGuiTreeNodeFlags flags = nodeFlags;

		// Render listComponents
		/**
		for (listComponent& c : componentList) {
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
					bool found = false;
					for (auto i : dataTabs) {
						if (i.name == c.index.name) {
							found = true; break;
						}
					}
					if (!found) {
						dataTab tab(c.index.name, c.index.f, c.meta);
						dataTabs.emplace_back(tab);
					}
				}
			}
		}
		/**/

		ClipboardContextMenu(clipboard, "Component", "Components");
	}
}

#endif