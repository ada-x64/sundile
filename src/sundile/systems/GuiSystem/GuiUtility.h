#ifndef GUI_UTILITY_H
#define GUI_UTILITY_H

#include "GuiTypes.h"

BEGIN_SYSTEM(GuiSystem)
	//-- [DESCRIPTION] Helper functions for GuiTypes{
	//-- HELPERS
	auto checkContext() {
		auto ctx = ImGui::GetCurrentContext();
		if (!ctx) { ctx = ImGui::CreateContext(); ImGui::SetCurrentContext(ctx); }
		return ctx;
	}
	void setState(const SmartEVW& evw, guiContainer& gui, const char* key, bool value) {
		gui.state[key] = value;
		GuiEvent ev(getRegistryByID(evw,currentScene), gui.state);
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
		return &(guiRegistry.get<guiContainer>(primaryGuiEntity));
	}

	//-- [SECTION] - Gui Trees
	static std::vector<int> toBeErased;
	static const ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
	static const ImGuiTreeNodeFlags selectedFlags = ImGuiTreeNodeFlags_Selected;
	static const ImGuiTreeNodeFlags leafFlags = ImGuiTreeNodeFlags_Leaf;

	template<typename T>
	listNodeRef<T> FindParent(guiTreeContainer<T>& container, listNodeRef<T>& target, std::function<void(const int i, listNodeRef<T>& parent)> callback = []() {}) {

		auto& contents = container.root->children;
		listNodeRef<T> parent;
		for (int i = 0; i < contents.size(); ++i) {

			parent = container.root;

			if (contents[i]->id == target->id) {
				callback(i, parent);
				return parent;
			}
			std::function<bool(listNodeRef<T>, bool)> recurse = [&](listNodeRef<T> p_node, bool found) {
				if (found || p_node->id == target->id)
					return true;

				parent = p_node; //should get the child node's first ancestor
				auto children = p_node->children;
				if (!children.empty()) {
					for (int j = 0; j < children.size(); ++j) {
						if (recurse(children[j], found)) {
							callback(j, parent);
							return true;
						}
					}
				}
				return found;
			};
			if (recurse(contents[i], false)) return parent;
		}
		return parent;
	}

	template<typename T>
	listNodeRef<T> RemoveFromTree(guiTreeContainer<T>& container, listNodeRef<T>& target) {
		return FindParent<T>(container, target, [](const int j, auto parent) {
			removeErase(parent->children, parent->children[j]);
			});
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
	void RenderListNode(listNodeRef<T>& p_node, guiTreeContainer<T>& tree) {
		static const auto setState = [](listNodeRef<T>& p_node) {
			//note: was using this to do the recomended mouseDelta method for rearranging items
			//p_node->state["hovered"] = ImGui::IsItemHovered();
			//p_node->state["active"] = ImGui::IsItemActive();
		};

		//TODO: Allow for more nuanced interaction
		//ie use IO to check mouse status
		static const auto doCallbacks = [&tree](listNodeRef<T>& p_node, listNodeRef<T>& parent) {
			auto& callbacks = tree.callbacks;
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
		static const auto doDragDrop = [&tree](listNodeRef<T>& p_node) {
			if (ImGui::BeginDragDropTarget()) {
				if (p_node->state["folder"]) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GUI_TREE_ELEMENT")) {
						//Add to folder
						//take payload node and place it as child of folder node
						//then remove it from the tree...?
						//search the tree for the element. remove it when found. then place it in the proper location.
						listNodeRef<T> data = std::make_shared<listNode<T>>(*(listNode<T>*)(payload->Data));
						if (p_node != data) {
							RemoveFromTree(tree, data);
							p_node->children.push_back(std::move(data));
						}
					}
					ClearGuiTreeSelectionState<T>(tree);
				}
				else {

				}
				ImGui::EndDragDropTarget();
			}
		};

		//recursively render tree
		static const std::function<void(listNodeRef<T>&, listNodeRef<T>&)> recurse = [&](listNodeRef<T>& p_node, listNodeRef<T> parent) {
			auto flags = (p_node->state["folder"] ? nodeFlags : leafFlags) | (p_node->state["selected"] ? selectedFlags : 0);
			if (ImGui::TreeNodeEx(p_node->name.c_str(), flags)) {
				for (int i = 0; i < p_node->children.size(); ++i) {
					recurse(p_node->children[i], p_node);
				}
				ImGui::TreePop();
			}
			setState(p_node);
			doCallbacks(p_node, parent);
			doDragDrop(p_node);
		};
		recurse(p_node, tree.root);
	}

	template<typename T>
	void RenderGuiTree(guiTreeContainer<T>& container) {
		auto contents = container.root->children;
		for (int i = 0; i < contents.size(); ++i) {
			RenderListNode<T>(contents[i], container);
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
		auto contents = tree.root->children;
		for (int i = 0; i < contents.size(); ++i) {
			ClearListNodeSelectionState(contents[i]);
		}
	}

END_SYSTEM

#include "GuiClipboard.h"

#endif