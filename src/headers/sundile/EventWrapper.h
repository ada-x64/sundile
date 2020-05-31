//--
//-- EventWrapper.h
//--
#include "Common.h"

#pragma once
#ifndef SUNDILE_ENTT_H
#define SUNDILE_ENTT_H

namespace sundile {
	//--
	//-- Entt Wrappers
	//--
	typedef std::shared_ptr<entt::registry> SmartRegistry;


	//--
	//-- Events
	//--

	enum class EventType { //Perhaps separate this into different enums?
		// Generic Events
		generic_draw,
		generic_game,
		generic_gui,
		generic_project,
		generic_render,
		generic_window,

		// Window Events
		cursorpos,
		framebufferSize,
		key,
		mousebtn,
		terminateWindow,
		updateWindow,
		window_finishTermination,

		// Sim Events

		// Render Events

		// Gui Events

		// Project Events

		// Count
		__COUNT
	};

	// To: Any
	// From: Any
	// Shorthand: "ev"
	// Event base class
	struct Event {
		EventType type;
	};


	// To: SimSystem, GuiSystem, ProjectSystem
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
	struct InputEvent : WindowEvent {
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

	};
	
	// To: RenderSystem
	// From: SimSystem
	// Shorthand: "rev"
	// Example: Send renderable data to RenderSystem
	struct RenderEvent : Event {

	};

	// To: GuiSystem
	// From: SimSystem
	// Shorthand: "uev"
	// Example: Send float to GUI for live editing.
	struct GuiEvent : Event {

	};

	// To: ProjectSystem
	// From: GuiSystem, SimSystem
	// Shorthand: "pev"
	// Example: Send relevant information to create snapshot, save or load project.
	struct ProjEvent : Event {

	};


	//--
	//-- Wrapper
	//--

	struct EventWrapper {
		entt::dispatcher dispatcher{};

		//--
		//-- Get Events 
		//-- 
		template <typename t>
		void getEvent(Event e);

		void step();
		void terminate(); //needed?

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
}
#endif