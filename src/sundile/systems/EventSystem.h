//--
//-- EventWrapper.h
//--
#include "../globals/Common.h"

#pragma once
#ifndef SUNDILE_ENTT_H
#define SUNDILE_ENTT_H

namespace sundile {

	//--
	//-- Wrappers
	//--

	struct EventWrapper {
		entt::dispatcher dispatcher{};
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
	struct initEvent {SmartEVW evw;	};
	struct preStepEvent :Event {};
	struct stepEvent :Event {};
	struct postStepEvent:Event {};
	struct terminateEvent :Event {};

	// WINDOW EVENTS

	struct WindowEvent : Event {GLFWwindow* window;};

	struct WindowInitEvent : WindowEvent {
		WindowInitEvent(GLFWwindow* window) { this->window = window; };
	};

	template <typename t>
	struct TypedWindowEvent : WindowEvent {std::vector<t> vals;};

	struct WindowInputEvent : WindowEvent {
		int key,
			scancode,
			action,
			mods;
	};

	struct WindowTerminateEvent : WindowEvent {};

	// SIM EVENTS
	// This includes GUI and Project events
	struct SimEvent : Event {
		SmartRegistry registry;
		float deltaTime;
		float currentTime;
		SimEvent(SmartRegistry registry, float deltaTime, float currentTime) : registry(registry), deltaTime(deltaTime), currentTime(currentTime) {};
		SimEvent(SmartRegistry registry) :registry(registry), deltaTime(-1), currentTime(-1) {};
		SimEvent() = default;
	};
	struct SimInitEvent : SimEvent {SmartEVW evw;};
	struct SimInputEvent : SimEvent {
		int key,
			scancode,
			action,
			mods;
	};
	struct SimStepEvent : SimEvent {};
	
	struct RenderEvent : SimEvent {};

	// GUI EVENTS
	// for interaction with defineGui()
	enum GuiStateKey {
		entityInspector,
		componentInspector,
		focusAny
	};

	struct GuiEventContent {
		GuiStateKey key;
		bool value;
		GuiEventContent(GuiStateKey k, const bool v) : key(k), value(v) {};
	};
	struct GuiEvent : SimEvent {
		 GuiEventContent payload;
		 GuiEvent(SimEvent ev, GuiEventContent payload) : SimEvent(ev), payload(payload) {};
	};
	struct RenderGuiEvent : SimEvent {};

	// PROJECT EVENTS
	struct ProjEvent : SimEvent {

	};



	namespace EventSystem {
		inline bool run = true;
		inline std::vector<SmartEVW> EVWs;

		SmartEVW create();

		void init(SmartEVW evw);
		void terminate(SmartEVW evw);

		void initAll();
		void updateAll();
		void terminateAll();
	}
}


#endif