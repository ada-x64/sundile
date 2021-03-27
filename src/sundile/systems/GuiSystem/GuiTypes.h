#ifndef GUI_TYPES_H
#define GUI_TYPES_H

SYSTEM(GuiSystem)
	//[SECTION] - GuiSystem Scope
	//[SECTION] - forward decl. & typedefs
		//-- General Containers
	struct guiContainer;
	//typedef std::map<std::string, bool> StateMap; //moved to general types
	typedef std::function<void(guiContainer&)> guiContainerFunc;

		//-- Inspector Trees
	template<typename T>
	struct listNode;
	template<typename T>
	using listNodeRef = std::shared_ptr<listNode<T>>;

	struct listEntity;
	struct listComponent;
	typedef std::vector<listEntity> guiEntityList;
	typedef std::vector<listComponent> guiComponentList;

		//-- Trees
	template <typename T>
	struct guiTreeContainer;

	template <typename T>
	using nodeEventCallback = std::function<void(listNodeRef<T>, guiTreeContainer<T>&)>;

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

	//[SECTION] - structs

	// Contains render functions
	struct guiIndex {
		std::string name = "(unregistered component(s))";
		guiRenderFunc f = nullRenderFunc; //by default, do nothing
		entt::id_type id = -1;
	};
	// Contains metas, which have the actual values to be passed to the function stored in the corresponding guiIndex
	struct listComponent {
		guiIndex index;
		guiMeta meta;
		std::string name;
		bool operator ==(listComponent other) { return this->index.id == other.index.id; }
		listComponent(const listComponent& other) : index(other.index), meta(other.meta), name(other.index.name) {};
		listComponent() = default;
	};
	// Contains components
	struct listEntity {
		std::string name = "(unset)";
		entt::entity entity = entt::null;
		guiComponentList componentList = {};
		bool operator ==(listEntity other) { return this->entity == other.entity; }
		listEntity(const listEntity& other) : entity(other.entity), name(other.name), componentList(other.componentList) {};
		listEntity() = default;
	};
	// Parent struct for listComponent and listEntity. Acts as a node in a tree.
	template <typename T>
	struct listNode {
		unsigned int id = std::time(nullptr); //uuid
		nodeList<T> children;
		std::string name = "(unset)";
		std::shared_ptr<T> content;
		StateMap state;
		listNode(T content) : content(std::make_shared<T>(content)), name(content.name) {};
	};
	//
	typedef std::map<ImGuiStyleVar, float> styleVarMap;

	// Contains GUI Windows
	static const guiContainerFunc nullContainerFunc = [](guiContainer&) -> void {};
	struct guiContainer {
		std::string name;
		StateMap state;
		guiContainerFunc renderFunc;
		ImGuiWindowFlags windowFlags;
		styleVarMap styleVars;
		ImVec2 size;

		guiContainer(const char* name = "UNDEFINED",
			guiContainerFunc renderFunc = nullContainerFunc,
			ImVec2 size = ImVec2(0.f, 0.f),
			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None,
			styleVarMap styleVars = {}
		)
			: name(name),
			renderFunc(renderFunc),
			size(size),
			windowFlags(windowFlags),
			styleVars(styleVars)
		{};
	};
	// General purpose clipboard
	template <typename T>
	struct guiClipboard {
		nodeList<T> selected;
		nodeList<T> list;
		StateMap state;
		listNodeRef<T> toBeRenamed;
		char namebuff[64];
	};
	// Wrapper for tabs. Assumes it will contain a child window
	template<typename T>
	struct guiTab {
		guiTreeContainer<T> treeContainer;
		std::vector<T>* data;
		size_t dataSize;
		guiTabFunc<T> renderfunc = [](guiContainer&,guiTreeContainer<T>&){};
		guiContainer container;
		std::string name = "";
		void render() {
			//update data
			auto& tree = treeContainer.root->children;
			//add missing values - this was added because initGuiFrontend was called before entityList was populated
			if (tree.empty() && data->size() != 0) {
				for (auto i = 0; i < data->size(); ++i) {
					T content = data->at(i);
					listNodeRef<T> node = std::make_shared<listNode<T>>(content);
					tree.push_back(std::move(node));
				}
			}
			else if (false) {// (tree.size() != data->size()) { //hopefully this should never run
				//for (auto i = 0; i < data->size(); ++i) {
				//	T value = data->at(i);
				//	auto found = std::find_if(tree.front(), tree.back(), [value](auto arg) {return arg.name == value.name; });
				//	if (found == tree.back()) {
				//		listNodeRef<T> node = std::make_shared<listNode<T>>(value);
				//		treeContainer.tree.push_back(std::move(node));
				//	}
				//}
			}

			//render
			if (ImGui::BeginTabItem(name.c_str(), &(container.state["open"]))) {
				if (ImGui::BeginChild(name.c_str(), ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
					renderfunc(container, treeContainer);
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}
		guiTab(std::string name, guiTabFunc<T> func, std::vector<T>* data) : name(name), renderfunc(func), data(data) {
			container.state["open"] = true;
			for (auto i = 0; i < data->size(); ++i) {
				T content = data->at(i);
				listNodeRef<T> node = std::make_shared<listNode<T>>(content);
				treeContainer.root->children.push_back(std::move(node));
			}
		};
	};
	struct dataTab {
		guiMeta data;
		guiRenderFunc renderfunc = nullRenderFunc;
		guiContainer container;
		std::string name = "";
		void render() {
			if (ImGui::BeginTabItem(name.c_str(), &(container.state["open"]))) {
				renderfunc(data);
				ImGui::EndTabItem();
			}
		}
		dataTab(std::string name, guiRenderFunc func, guiMeta& data) : name(name), renderfunc(func), data(data) {
			container.state["open"] = true;
		};
	};

	//[SECTION] - Namespace Globals

	typedef unsigned int windowID;
	typedef unsigned int sceneID;
	typedef unsigned int evwID;

	// Member Variables
	static std::vector<guiIndex> guiIndices;
	static std::vector<guiMeta> metaList;
	static std::vector<listEntity> entityList;
	static std::map<const char*, void*> clipboardList;
	static const listEntity nullListEntity;
	static entt::registry guiRegistry;
	static entt::entity primaryGuiEntity;

	static windowID currentWindow = -1;
	static sceneID currentScene = -1;
	static evwID currentEVW = -1;

END_SYSTEM
#endif