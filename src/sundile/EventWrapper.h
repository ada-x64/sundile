//--
//-- EventWrapper.h
//--
#include "Common.h"

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

	enum class EventType { //Perhaps separate this into different enums?
		// Generic Events
		generic_draw,
		generic_game,
		generic_gui,
		generic_project,
		generic_render,
		generic_window,

		// Main Loop Events
		init,
		emplace,

		preStep,
		step,
		postStep,

		preRender,
		render,
		postRender,

		preDrawGui,
		drawGui,
		postDrawGui,

		terminate,

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
	// Example: Main loop events (carry no information)
	// Event base class
	struct Event {
		EventType type;
	};

	struct initEvent {
		EventType type = EventType::init;
		SmartEVW evw;	
	};

	struct preStepEvent		{ EventType type = EventType::preStep; };
	struct stepEvent		{ EventType type = EventType::step; };
	struct postStepEvent	{ EventType type = EventType::postStep; };
	
	struct preRenderEvent	{ EventType type = EventType::preRender; };
	struct renderEvent		{ EventType type = EventType::render; };
	struct postRenderEvent	{ EventType type = EventType::postRender; };

	struct preDrawGuiEvent	{ EventType type = EventType::preDrawGui; };
	struct drawGuiEvent		{ EventType type = EventType::drawGui; };
	struct postDrawGuiEvent { EventType type = EventType::postDrawGui; };

	struct terminateEvent	{ EventType type = EventType::terminate; };

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
		double deltaTime;
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



	namespace EventSystem {
		inline bool run = true;
		inline std::vector<SmartEVW> EVWs;

		SmartEVW create();

		void init(SmartEVW evw);
		void update(SmartEVW evw);
		void terminate(SmartEVW evw);

		void initAll();
		void updateAll();
		void terminateAll();
	}
}
#endif