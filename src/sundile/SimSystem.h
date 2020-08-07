//--
//-- SimSystem.h
//--
#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include "EventWrapper.h"
#include "Components.h"
namespace sundile {
	//Simulation struct
	struct Sim {
		SmartEVW evw;
		SmartRegistry registry;

		//-- When incrementing, "n * deltaTime" means "n per second".
		float deltaTime = 0.f;
		float lastTime = 0.f;
	};

	typedef std::shared_ptr<Sim> SmartSim;

	namespace SimSystem {
		inline std::vector<SmartSim> sims = std::vector<SmartSim>();

		SmartSim init(SmartEVW evw);
		void update(SmartSim game);
		void updateAll();
	};
}

#endif