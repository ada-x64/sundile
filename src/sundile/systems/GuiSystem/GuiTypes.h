#ifndef GUI_TYPES_H
#define GUI_TYPES_H
namespace sundile {
	// Contains typeinfo for registered components.
	struct guiMeta {
		void* ref;
		entt::id_type id = -1;
		entt::entity entt;
	};
	// Contains the Dear IMGUI instructions for registered components
	typedef std::function<void(const guiMeta&)> guiRenderFunc;

	// Null members
	static const guiMeta nullMeta;
	static const guiRenderFunc nullRenderFunc = [](const guiMeta&) { ImGui::Text("(empty)"); };
}
namespace sundile::GuiSystem {
	//[SECTION] - forward decl. & typedefs
		// General Containers
	struct guiContainer;
	typedef std::map<std::string, bool> guiStateMap;
	typedef std::function<void(guiContainer&)> guiContainerFunc;

		// Inspector Trees
	template<typename T>
	struct listNode;
	struct listEntity;
	struct listComponent;
	template<typename T>
	using guiTree = std::vector<listNode<T>*>;
	typedef std::vector<listEntity> guiEntityList;
	typedef std::vector<listComponent> guiComponentList;

		// Inspector Tabs
	template<typename T>
	using guiTabFunc = std::function<void(guiContainer&, guiTree<T>&)>;
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
		guiTree<T> children;
		std::string name = "(unset)";
		std::shared_ptr<T> content;
		guiStateMap state;
		listNode(T content) : content(std::make_shared<T>(content)), name(content.name) {};
	};
	// Contains primary GUI
	static const guiContainerFunc nullContainerFunc = [](guiContainer&) -> void {};
	struct guiContainer {
		std::string name;
		guiStateMap state;
		guiContainerFunc renderFunc;
		ImGuiWindowFlags windowFlags;
		ImVec2 size;
		guiContainer(const char* name = "UNDEFINED", guiContainerFunc renderFunc = nullContainerFunc, ImVec2 size = ImVec2(0.f, 0.f), ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None)
			: name(name), renderFunc(renderFunc), size(size), windowFlags(windowFlags) {};
	};
	// General purpose clipboard
	template <typename T>
	struct guiClipboard {
		std::vector<T*> selected;
		std::vector<T> list;
		guiStateMap state;
		T* toBeRenamed = nullptr;
		char namebuff[64];
	};
	// Wrapper for tabs. Assumes it will contain a child window
	template<typename T>
	struct guiTab {
		guiTree<T> tree;
		std::vector<T>* data;
		guiTabFunc<T> renderfunc = [](){};
		guiContainer container;
		std::string name = "";
		void render() {
			if (ImGui::BeginTabItem(name.c_str(), &(container.state["open"]))) {
				if (ImGui::BeginChild(name.c_str(), ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
					renderfunc(container, tree);
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}
		guiTab(std::string name, guiTabFunc<T> func, std::vector<T>* data) : name(name), renderfunc(func), data(data) {
			container.state["open"] = true;
			for (auto i = 0; i < data->size(); ++i) {
				T content = data->at(i);
				listNode<T>* node = new listNode<T>(content);
				tree.push_back(std::move(node));
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
	typedef unsigned int simID;
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
	static simID currentSim = -1;
	static evwID currentEVW = -1;
}
#endif