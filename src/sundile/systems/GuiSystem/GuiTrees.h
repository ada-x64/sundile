#ifndef SUNDILE_GUI_TREES
#define SUNDILE_GUI_TREES

SYSTEM(GuiSystem)

enum guiTreeInputEvent {
	leftClick,
	rightClick,
	middleClick,

	leftDoubleClick,
	rightDoubleClick,
	middleDoubleClick,

	leftHeld,
	rightHeld,
	middleHeld,

	leftRelease,
	rightRelease,
	middleRelease,

	COUNT
};


//-- Inspector Trees
template<typename T>
struct listNode;
template<typename T>
using listNodeRef = std::shared_ptr<listNode<T>>;

//-- Trees
template <typename T>
struct guiTreeContainer;

template <typename T>
using nodeEventCallback = std::function<void(listNodeRef<T>, guiTreeContainer<T>&)>;

struct listEntity;
struct listComponent;
typedef std::vector<listEntity> guiEntityList;
typedef std::vector<listComponent> guiComponentList;

template<typename T>
using guiTreeCallbackMap = std::map<guiTreeInputEvent, nodeEventCallback<T>>;

template<typename T>
using nodeList = std::vector<listNodeRef<T>>;

template<typename T>
struct guiTreeContainer {
	listNodeRef<T> root;
	guiTreeCallbackMap<T> callbacks;
	nodeEventCallback<T> nullCallback = [](listNodeRef<T>, guiTreeContainer<T>&) {};
	guiTreeContainer(std::vector<T> list) {
		T content;
		root = std::make_shared<listNode<T>>(content);
		for (auto i : list) {
			root->children.emplace_back(std::make_shared<listNode<T>>(i));
		}
		for (int i = 0; i < guiTreeInputEvent::COUNT; ++i) {
			callbacks[guiTreeInputEvent(i)] = nullCallback;
		}

	}
	guiTreeContainer() {
		T content;
		root = std::make_shared<listNode<T>>(content);
		for (int i = 0; i < guiTreeInputEvent::COUNT; ++i) {
			callbacks[guiTreeInputEvent(i)] = nullCallback;
		}
	};
};


// Inspector Tabs
template<typename T>
using guiTabFunc = std::function<void(guiContainer&, guiTreeContainer<T>&)>;

template<typename T>
struct guiTab;
typedef guiTab<listEntity> entityTab;
typedef guiTab<listComponent> componentTab;



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
#endif