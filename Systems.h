//--
//-- Systems.h
//--
#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include "Components.h"
namespace sundile {
	namespace Systems {
		//--
		//-- Data Members
		//--

		//-- Structs
		struct keyEvent { int key, scancode, action, mods; };

		//-- Delta time
		//-- When incrementing, "n * deltaTime" means "n per second".
		inline float deltaTime = 0.f;
		inline float lastTime = 0.f;

		//-- entt Data
		inline entt::dispatcher dispatcher{};
		inline entt::registry registry;

		//-- misc
		inline bool windowShouldClose = false;

		//--
		//-- Function Members
		//--

		keyEvent handleKeyEvents(const keyEvent& event);

		void connectListeners();

		void setDependencies();

		void step();
	};
}

#endif