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
	template <typename T>
	struct Event { T member; };

	template <typename T>
	struct InitEvent : Event<T> {};
	template <typename T>
	struct ReadyEvent : Event<T> {};
	template <typename T>
	struct PreStepEvent : Event<T> {};
	template <typename T>
	struct StepEvent : Event<T> {};
	template <typename T>
	struct PostStepEvent : Event<T> {};
	template <typename T>
	struct TerminateEvent : Event<T> {};

	template <typename T>
	struct ActivateEvent : Event<T> {};
	template <typename T>
	struct DeactivateEvent : Event<T> {};

	template <typename T>
	struct CreateEvent : Event<T> {};
	template <typename T>
	struct DestroyEvent : Event<T> {};

	template <typename T>
	struct ChangeEvent : Event<T> {};

	template <typename T>
	struct SerializeEvent : Event<T> {};
	template <typename T>
	struct DeserializeEvent : Event<T> {};

	template <typename T>
	struct RenderEvent : Event<T> {};

	template <typename T>
	struct InputEvent : Event<T> {};
}
#endif