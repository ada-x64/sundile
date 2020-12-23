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

	// SIM EVENTS
	// This includes GUI and Project events
	struct SimEvent : Event {
		unsigned int id;
		SmartRegistry registry;
		float deltaTime;
		float currentTime;
		SimEvent(SmartRegistry registry, float deltaTime, float currentTime) : registry(registry), deltaTime(deltaTime), currentTime(currentTime) {};
		SimEvent(SmartRegistry registry) :registry(registry), deltaTime(-1), currentTime(-1) {};
		SimEvent() = default;
	};
	struct SimInitEvent : SimEvent { SmartEVW evw; };
	struct SimInputEvent : SimEvent {
		int key,
			scancode,
			action,
			mods;
	};
	struct SimStepEvent : SimEvent {};

	struct RenderEvent : SimEvent {};

	struct registryWrapper {
		entt::registry* registry;
	};
	struct SimRegistryQuery : Event {
		unsigned int id;
		registryWrapper* wrapper;
	};

	// GUI EVENTS
	// for interaction with defineGui()
	enum GuiStateKey {
		entityInspector,
		componentInspector,
		focusAny
	};
	struct GuiEventContent {
		const char* key;
		bool value;
		GuiEventContent(const char* k, const bool v) : key(k), value(v) {};
	};
	struct GuiEvent : Event {
		entt::registry* registry;
		GuiEventContent payload;
		GuiEvent(entt::registry* registry, GuiEventContent payload) : registry(registry), payload(payload) {};
	};
	struct RenderGuiEvent : SimEvent {};

	// PROJECT EVENTS
	struct ProjEvent : SimEvent {

	};
}
#endif