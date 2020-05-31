//--
//-- GameSystem.h
//--
#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include "EventWrapper.h"
#include "Components.h"
#include "AllComponents.h"
namespace sundile {

	struct Game {
		SmartEVW evw;
		entt::registry registry;

		//-- When incrementing, "n * deltaTime" means "n per second".
		float deltaTime = 0.f;
		float lastTime = 0.f;
	};

	namespace GameSystem {
		//--
		//-- Function Members
		//--
		Game init(SmartEVW evw);
		void step(Game& game, SmartEVW evw);
		//void render(SmartEVW evw);
		//void terminate(SmartEVW evw);
	};
}

#endif