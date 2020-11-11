#ifndef GUI_TYPES_H
#define GUI_TYPES_H
namespace sundile {
	// for interaction with defineGui()
	enum GuiStateKey {
		entityInspector,
		componentInspector,
		focusAny
	};
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
	};
	// Contains components
	struct listEntity {
		entt::entity entity = entt::null;
		std::string name = "(unset entity)";
		std::vector<listComponent> componentList = {};
		bool operator ==(listEntity other) { return this->entity == other.entity; }
	};
	// Contains primary GUI
	struct guiContainer {
		std::map<const GuiStateKey, bool> state;
		std::function<void()> renderFunc;
		guiContainer() : renderFunc([]() {}) {};
		guiContainer(std::function<void()> renderFunc) : renderFunc(renderFunc) {};
	};

	typedef unsigned int windowID;
	typedef unsigned int simID;
	typedef unsigned int evwID;

	// Member Variables
	static std::vector<guiIndex> guiIndices;
	static std::vector<guiMeta> metaList;
	static std::vector<listEntity> entityList;
	static const listEntity nullListEntity;
	static entt::registry guiRegistry;

	static windowID currentWindow = -1;
	static simID currentSim = -1;
	static evwID currentEVW = -1;
}
#endif