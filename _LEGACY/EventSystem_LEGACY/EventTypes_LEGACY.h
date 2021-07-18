#ifndef SUNDILE_EVENT_TYPES
#define SUNDILE_EVENT_TYPES
namespace sundile {

	//--
	//-- Wrappers
	//--

	struct EventWrapper {
		entt::dispatcher dispatcher{};
		unsigned int id;
	};
	struct EventWrapperDestroyer {
		void operator()(EventWrapper* evw) {
			delete evw;
		}
	};

	typedef std::shared_ptr<EventWrapper> SmartEVW;
	typedef std::shared_ptr<entt::registry> SmartRegistry;

	// GENERIC EVENTS
	struct Event {};
	struct InitEvent { SmartEVW evw; };
	struct PreStepEvent :Event {};
	struct StepEvent :Event {};
	struct PostStepEvent :Event {};
	struct TerminateEvent :Event {};

	template <typename T>
	struct ActivateEvent : Event { T member; };
	template <typename T>
	struct DeactivateEvent : Event { T member; };
	template <typename T>
	struct CreateEvent : Event { T member; };
	template <typename T>
	struct DestroyEvent : Event { T member; };
	template <typename T>
	struct ChangeEvent : Event { T member; };
	template <typename T>
	struct SerializeEvent : Event { T member; };
	template <typename T>
	struct DeserializeEvent : Event { T member; };


	// WINDOW EVENTS
	struct WindowEvent : Event { unsigned int id; };
	struct WindowInitEvent : WindowEvent {
		GLFWwindow* window;
		WindowInitEvent(unsigned int id, GLFWwindow* window) { this->id = id; this->window = window; };
	};
	template <typename t>
	struct TypedWindowEvent : WindowEvent { std::vector<t> vals; };
	struct WindowInputEvent : WindowEvent {
		int key,
			scancode,
			action,
			mods;
	};
	struct WindowTerminateEvent : WindowEvent {};

	struct WindowSizeQuery : WindowEvent {
		Vec2* size;
	};

	// SCENE EVENTS
	// This includes GUI and Project events
	struct SceneEvent : Event {
		unsigned int id;
		SmartRegistry registry;
		float deltaTime;
		float currentTime;
		SceneEvent(SmartRegistry registry, float deltaTime, float currentTime) : registry(registry), deltaTime(deltaTime), currentTime(currentTime) {};
		SceneEvent(SmartRegistry registry) :registry(registry), deltaTime(-1), currentTime(-1) {};
		SceneEvent() = default;
	};
	struct SceneInitEvent : SceneEvent { SmartEVW evw; };
	struct SceneInputEvent : SceneEvent {
		int key,
			scancode,
			action,
			mods;
	};
	struct SceneStepEvent : SceneEvent {};

	struct RenderEvent : SceneEvent {};

	struct registryWrapper {
		entt::registry* registry;
	};
	struct SceneRegistryQuery : Event {
		unsigned int id;
		registryWrapper* wrapper;
	};

	struct ScenePlayEvent : SceneEvent {};
	struct ScenePauseEvent : SceneEvent {};

	// GUI EVENTS
	// for interaction with defineGui()
	struct GuiEvent : Event {
		SmartRegistry registry;
		StateMap payload;
		GuiEvent(SmartRegistry registry, StateMap payload) : registry(registry), payload(payload) {};
	};
	struct RenderGuiEvent : SceneEvent {};

	// PROJECT EVENTS
	struct ProjEvent : Event {};
	struct ProjSaveEvent : ProjEvent {};
	struct ProjLoadEvent : ProjEvent {};
	struct ProjInitEvent : ProjEvent {};

}
#endif