
#ifndef SUNDILE_SCENE_TYPES
#define SUNDILE_SCENE_TYPES

#include "../../globals/Common.h"

COMPONENT(Scene)
	unsigned int id = -1;
	std::string name = "NEW SCENE";
	SmartRegistry registry;
	Renderer renderer;
	Input input;
	guiEntityList entityList;

	std::function<void(std::shared_ptr<Scene>&)> open = [](std::shared_ptr<Scene>&) {};
	std::function<void(std::shared_ptr<Scene>&)> close = [](std::shared_ptr<Scene>&) {};

	//-- When incrementing, "n * deltaTime" means "n per second".
	float deltaTime = 0.f;
	float lastTime = 0.f;
	float currentTime = 0.0f;

	std::vector<guiMeta> guiMetas;
END_COMPONENT

namespace sundile {
	typedef std::shared_ptr<Components::Scene> SmartScene;
}

#endif