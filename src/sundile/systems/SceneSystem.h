//--
//-- SceneSystem.h
//--
#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include "EventSystem/EventSystem.h"
namespace sundile {
	//Simulation struct
	struct Scene {
		unsigned int id;
		SmartEVW evw;
		SmartRegistry registry;

		//-- When incrementing, "n * deltaTime" means "n per second".
		float deltaTime = 0.f;
		float lastTime = 0.f;
		float currentTime = 0.0f;
	};

	typedef std::shared_ptr<Scene> SmartScene;

	namespace SceneSystem {
		inline std::vector<SmartScene> scenes = std::vector<SmartScene>();

		SmartScene init(SmartEVW evw);
		void update(SmartScene game);
		void updateAll();
	};
}

#endif