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


	//--
	//-- Events
	//--

	// To: Any
	// From: Any
	// Shorthand: "ev"
	// Example: Main loop events (carry no information)
	// Event base class
	struct Event {};

	struct initEvent {
		SmartEVW evw;	
	};

	struct preStepEvent :Event {};
	struct stepEvent :Event {};
	struct postStepEvent:Event {};

	struct terminateEvent :Event {};

	// To: SimSystem, GuiSystem, ProjectSystem, WindowSystem (termination)
	// From: WindowSystem
	// Shorthand: "wev"
	// Example: Key event, window resize event -- anything from GLFW
	// Types: generic_window, terminateWindow, updateWindow
	struct WindowEvent : Event {
		GLFWwindow* window;
	};

	// Internal, generic event type for GLFW events that aren't input-related
	// Types: cursorpos, framebufferSize
	template <typename t>
	struct TypedWindowEvent : WindowEvent {
		std::vector<t> vals;
	};

	// To: SimSystem, GuiSystem, ProjectSystem
	// From: WindowSystem
	// Shorthand: "iev"
	// Example: Key input, mouse position update, mouse button clicked, etc.
	// Types: key, mouseBtn, ...
	struct WindowInputEvent : WindowEvent {
		int key,
			scancode,
			action,
			mods;
	};

	// To: WindowSystem
	// From: GuiSystem, RenderSystem
	// Shorthand: "dev"
	// Example: Draw GUI or rendered scene.
	struct DrawEvent : Event {

	};

	// To: SimSystem
	// From: Component systems, GuiSystem
	// Shorthand: "gev"
	// Example: Send float modified in GUI to SimSystem; For component systems: idk yet lol
	struct SimEvent : Event {
		SmartRegistry registry;
		float deltaTime;
		float currentTime;
	};
	struct SimInitEvent : SimEvent {
		SmartEVW evw;
	};
	struct SimInputEvent : SimEvent {
		int key,
			scancode,
			action,
			mods;
	};
	struct SimStepEvent : SimEvent {};
	
	// To: RenderSystem
	// From: SimSystem
	// Shorthand: "rev"
	// Example: Send renderable data to RenderSystem
	struct RenderEvent : SimEvent {};

	// To: GuiSystem
	// From: SimSystem
	// Shorthand: "uev"
	// Example: Send float to GUI for live editing.
	union GuiEventContent {
		bool b;
	};
	struct GuiEvent : SimEvent {
		GuiEventContent content;
	};
	struct RenderGuiEvent : SimEvent {};

	// To: ProjectSystem
	// From: GuiSystem, SimSystem
	// Shorthand: "pev"
	// Example: Send relevant information to create snapshot, save or load project.
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