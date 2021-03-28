#pragma once
//-- System
#include "systems/Systems.h"

namespace sundile {
	SmartEVW init() {

		//Initialize
		SmartEVW evw = Systems::EventSystem::create();
		SmartWindow winc = Systems::WindowSystem::initWindowedFullscreen(evw);
		Project project(evw);
		Systems::init(evw, project);
		Systems::SceneSystem::init(evw);
		Systems::GuiSystem::init(evw);
		winc->title = "sundile";

#ifdef SUNDILE_EXPORT
		winc->guiEnabled = false;
#else
		winc->guiEnabled = true;
#endif

		glfwSetWindowSizeLimits(winc->window, winc->WIDTH, winc->HEIGHT, winc->WIDTH, winc->HEIGHT);

		return evw;
	}

	void mainLoop() {
		//main loop
		Systems::EventSystem::initAll();

		while (Systems::EventSystem::run) {
			Systems::EventSystem::updateAll();
		}

		Systems::EventSystem::terminateAll();
	}
}