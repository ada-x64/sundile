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
		bool operator ==(listComponent other) { return this->index.id == other.index.id; }
		listComponent(const listComponent& other) : index(other.index), meta(other.meta) {};
		listComponent() = default;
	};
	// Contains components
	struct listEntity {
		entt::entity entity = entt::null;
		std::string name = "(unset entity)";
		std::vector<listComponent> componentList = {};
		bool operator ==(listEntity other) { return this->entity == other.entity; }
		listEntity(const listEntity& other) : entity(other.entity), name(other.name), componentList(other.componentList) {};
		listEntity() = default;
	};
	// Contains primary GUI
	struct guiContainer;
	typedef std::map<std::string, bool> guiStateMap;
	typedef std::function<void(guiContainer&)> guiContainerFunc;
	static const guiContainerFunc nullContainerFunc = [](guiContainer&) -> void {};
	struct guiContainer {
		std::string name;
		guiStateMap state;
		guiContainerFunc renderFunc;
		guiContainer(const char* name = "UNDEFINED", guiContainerFunc renderFunc = nullContainerFunc) : name(name), renderFunc(renderFunc) {};
		guiContainer() = default;
	};

	template <typename T>
	struct guiClipboard {
		std::vector<T*> selected;
		std::vector<T> list;
		guiStateMap state;
		T* toBeRenamed = nullptr;
		char namebuff[64];
	};

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