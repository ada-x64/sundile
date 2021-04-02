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

	//EVW for use by Sundile internal systems.
	typedef std::shared_ptr<EventWrapper> SmartEVW;
	//EVW for use by component systems.
	typedef std::weak_ptr<EventWrapper> SafeEVW;

	//Registry for use by Sundile internal systems.
	typedef std::shared_ptr<entt::registry> SmartRegistry;
	//Registry for component systems.
	typedef std::weak_ptr<entt::registry> SafeRegistry;


	// GENERIC EVENTS
	struct Event {};
	struct initEvent { SmartEVW evw; };
	struct preStepEvent :Event {};
	struct stepEvent :Event {};
	struct postStepEvent :Event {};
	struct terminateEvent :Event {};

	template <typename T>
	struct activateEvent : Event {T member};
	template <typename T>
	struct deactivateEvent : Event { T member };
	template <typename T>
	struct createEvent : Event { T member };
	template <typename T>
	struct destroyEvent : Event { T member };
	template <typename T>
	struct changeEvent : Event { T member };
	template <typename T>
	struct serializeEvent : Event { T member };
	template <typename T>
	struct deserializeEvent : Event { T member };


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

	struct SceneSerializeEvent : SceneEvent {};
	struct SceneDeserializeEvent : SceneEvent {};
	struct SceneActivateEvent : SceneEvent {}; //fired when scene is made active
	struct SceneDeactivateEvent : SceneEvent {}; //fired when scene is no longer active 
	struct SceneCreateEvent : SceneEvent {};
	struct SceneDestroyEvent : SceneEvent {};

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