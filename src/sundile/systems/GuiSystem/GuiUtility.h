#ifndef GUI_UTILITY_H
#define GUI_UTILITY_H

#include "GuiTypes.h"

//-- [DESCRIPTION] Helper functions for GuiTypes
namespace sundile::GuiSystem {
	//-- HELPERS
	auto checkContext() {
		auto ctx = ImGui::GetCurrentContext();
		if (!ctx) { ctx = ImGui::CreateContext(); ImGui::SetCurrentContext(ctx); }
		return ctx;
	}
	void setState(const SmartEVW& evw, guiContainer& gui, const char* key, bool value) {
		gui.state[key] = value;
		GuiEvent ev(getRegistryByID(evw,currentScene), { key, value });
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

	//-- [SECTION] - Gui Trees
	static std::vector<int> toBeErased;
	static const ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
	static const ImGuiTreeNodeFlags selectedFlags = ImGuiTreeNodeFlags_Selected;
	static const ImGuiTreeNodeFlags leafFlags = ImGuiTreeNodeFlags_Leaf;

	template<typename T>
	bool RemoveFromTree(guiTreeContainer<T>& container, listNodeRef<T> target) {
		auto& tree = container.tree;
		for (int i = 0; i < tree.size(); ++i) {
			if (tree[i]->name == target->name) {
				removeErase(tree, tree[i]);
				return true;
			}
			std::function<bool(listNodeRef<T>, bool)> recurse = [&](listNodeRef<T> p_node, bool found) {
				if (found || p_node->name == target->name) return true;

				auto children = p_node->children;
				if (!children.empty()) {
					for (int j = 0; j < children.size(); ++j) {
						if (recurse(children[j], found)) {
							removeErase(children, children[j]);
							return true;
						}
					}
				}
				return found;
			};
			if (recurse(tree[i], false)) return true;
		}
		return false;
	}

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
	void RenderListNode(listNodeRef<T> p_node, guiTreeContainer<T>& tree) {
		auto io = ImGui::GetIO();
		auto& callbacks = tree.callbacks;
		//TODO: Allow for more nuanced interaction
		//ie use IO to check mouse status
		auto doCallbacks = [&]() {
			if (ImGui::IsItemClicked(0)) {
				callbacks[leftClick](p_node, tree);
			}
			if (ImGui::IsItemClicked(1)) {
				callbacks[rightClick](p_node, tree);
			}
			if (ImGui::IsItemClicked(2)) {
				callbacks[middleClick](p_node, tree);
			}


			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("GUI_TREE_ELEMENT", p_node.get(), sizeof(listNode<T>));
				ImGui::Text("Move Item...");
				ImGui::EndDragDropSource();
			}
		};

		//TODO: Make this actually do something
		auto doDragDrop = [&](bool folder) {
			if (!folder) {
				if (ImGui::BeginDragDropTarget()) {
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GUI_TREE_ELEMENT");
					if (payload) {
						//Reorder element
						listNodeRef<T> data = std::make_shared<listNode<T>>(*(listNode<T>*)(payload->Data));
						bool found = RemoveFromTree(tree, data);
					}
					ClearGuiTreeSelectionState<T>(tree);
					ImGui::EndDragDropTarget();
				}
			}
			else {
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GUI_TREE_ELEMENT")) {
						//Add to folder
						//take payload node and place it as child of folder node
						//then remove it from the tree...?
						//search the tree for the element. remove it when found. then place it in the proper location.
						listNodeRef<T> data = std::make_shared<listNode<T>>(*(listNode<T>*)(payload->Data));
						RemoveFromTree(tree, data);
						p_node->children.push_back(std::move(data));
					}
					ClearGuiTreeSelectionState<T>(tree);
					ImGui::EndDragDropTarget();
				}
			}
		};

		//recursively render tree
		//\TODO: Stack error on opening folders
		std::function<void(listNodeRef<T>)> recurse = [&](listNodeRef<T> p_node) {
			auto flags = (p_node->state["folder"] ? nodeFlags : leafFlags) | (p_node->state["selected"] ? selectedFlags : 0);
			if (ImGui::TreeNodeEx(p_node->name.c_str(), flags)) {
				for (int i = 0; i < p_node->children.size(); ++i) {
					recurse(p_node->children[i]);
				}
				ImGui::TreePop();
			}
			doCallbacks();
			doDragDrop(p_node->state["folder"]);
		};
		recurse(p_node);
	}

	template<typename T>
	void RenderGuiTree(guiTreeContainer<T>& container) {
		for (int i = 0; i < container.tree.size(); ++i) {
			RenderListNode<T>(container.tree[i], container);
		}
	}

	template<typename T>
	void ClearListNodeSelectionState(listNodeRef<T> p_node) {
		p_node->state["selected"] = false;
		for (int i = 0; i < p_node->children.size(); ++i) {
			ClearListNodeSelectionState<T>(p_node->children[i]);
		}
	}

	template <typename T>
	void ClearGuiTreeSelectionState(guiTreeContainer<T>& tree) {
		for (int i = 0; i < tree.tree.size(); ++i) {
			ClearListNodeSelectionState(tree.tree[i]);
		}
	}
}

#include "GuiClipboard.h"

#endif