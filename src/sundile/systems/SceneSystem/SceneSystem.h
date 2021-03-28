//--
//-- SceneSystem.h
//--
#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include "../EventSystem/EventSystem.h"

COMPONENT(Scene)
	unsigned int id = -1;
	SmartEVW evw;
	SmartRegistry registry;
	std::function<void(SmartRegistry&)> open = [](SmartRegistry&) {};
	std::function<void(SmartRegistry&)> close = [](SmartRegistry&) {};

	//-- When incrementing, "n * deltaTime" means "n per second".
	float deltaTime = 0.f;
	float lastTime = 0.f;
	float currentTime = 0.0f;
END_COMPONENT

namespace sundile {
	typedef std::shared_ptr<Scene> SmartScene;

	//--
	//-- Coord map
	entt::entity addCoords(sundile::SmartRegistry registry) {
		/**/
		using namespace Systems;
		auto eCoords = registry->create();
		Vertex v0{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vx{ glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vy{ glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vz{ glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) };
		Vertex vnx{ glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vny{ glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vnz{ glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) };
		Mesh coords = Mesh({ v0, vx, vy, vz, vnx, vny, vnz }, { 0,1,0, 0,2,0, 0,3,0, 0,4,0, 0,5,0, 0,6,0 }, {});
		registry->emplace<Mesh>(eCoords, coords);
		return eCoords;
		/**/
	}
}

SYSTEM(SceneSystem)
	static std::vector<SmartScene> scenes = std::vector<SmartScene>();
	static SmartScene currentScene;


	//--
	//-- Main Loop
	//--
	void update() {
		SmartScene& scene = currentScene;
		//-- Time
		scene->currentTime = glfwGetTime();
		scene->deltaTime = scene->currentTime - scene->lastTime;
		scene->lastTime = scene->currentTime;

		SmartEVW evw = scene->evw;

		//-- Update Events
		SceneStepEvent sev;
		sev.registry = scene->registry;
		sev.deltaTime = scene->deltaTime;
		sev.currentTime = scene->currentTime;
		evw->dispatcher.trigger<SceneStepEvent>(sev);

		RenderEvent rev;
		rev.registry = scene->registry;
		rev.deltaTime = scene->deltaTime;
		rev.currentTime = scene->currentTime;
		evw->dispatcher.trigger<RenderEvent>(rev);

		RenderGuiEvent gev;
		gev.registry = scene->registry;
		gev.deltaTime = scene->deltaTime;
		gev.currentTime = scene->currentTime;
		evw->dispatcher.trigger<RenderGuiEvent>(gev);
	}

	void sceneRegistryQuery(const SceneRegistryQuery& srq) {
		for (auto scene : scenes) {
			if (scene->id == srq.id) {
				srq.wrapper->registry = scene->registry.get();
			}
		}
	}

	//Parses GLFW key events and passes appropriate information to other functions.
	void handleInput(const WindowInputEvent& wev) {
		//-- Emergency Exit
		if (wev.key == GLFW_KEY_ESCAPE) {
			printf("Exiting on Keypress: Esc");
			exit(EXIT_FAILURE);
		}

		for (auto scene : scenes) {
			SceneInputEvent ev;
			ev.registry = scene->registry;
			ev.deltaTime = scene->deltaTime;
			ev.key = wev.key;
			ev.scancode = wev.scancode;
			ev.action = wev.action;
			ev.mods = wev.mods;
			scene->evw->dispatcher.trigger<SceneInputEvent>(ev);
		}
	}

	SmartScene create(SmartEVW evw) {
		// Initialize
		SmartScene scene = std::make_shared<Scene>();
		scene->registry = std::make_shared<entt::registry>();
		scene->evw = evw;
		scene->id = rand();

		SceneInitEvent ev;
		ev.id = scene->id;
		ev.evw = evw;
		ev.registry = scene->registry;
		evw->dispatcher.trigger<SceneInitEvent>(ev);

		// Required entities
		auto eRenderer = scene->registry->create();
		scene->registry->emplace<Renderer>( eRenderer, RenderSystem::create());

		auto eCam = scene->registry->create();
		scene->registry->emplace<Camera>(eCam);

		addCoords(scene->registry);

		//Add to scenes
		scenes.push_back(scene);

		//Ensure scene registry isn't empty
		if (currentScene.use_count() == 0) {
			currentScene.reset(scene.get());
		}


		return scene;
	}

	void init(SmartEVW evw) {
		// Connect event listeners
		evw->dispatcher.sink<stepEvent>().connect<update>();
		evw->dispatcher.sink<WindowInputEvent>().connect<handleInput>();
		//evw->dispatcher.sink<initEvent>().connect<catchInit>();
		evw->dispatcher.sink<SceneRegistryQuery>().connect<sceneRegistryQuery>();

		//Default scene
		create(evw);
	}


	void closeScene(CloseSceneEvent ev) {
		for (auto& scene : scenes) {
			if (scene->id == ev.id) {
				scene->close(scene->registry);
				break;
			}
		}
		currentScene = nullptr;
	}
	void openScene(OpenSceneEvent ev) {
		if (currentScene->id != -1)
			currentScene->close(currentScene->registry);

		for (auto& scene : scenes) {
			if (scene->id == ev.id) {
				scene->open(scene->registry);
				currentScene = scene;
				break;
			}
		}
	}

	//\TODO: Start implementing editor tools
	void parseGuiState(std::map<std::string, bool>& map) {

	}

END_SYSTEM

#endif