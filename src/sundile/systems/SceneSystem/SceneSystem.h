//--
//-- SceneSystem.h
//--
#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include "../EventSystem/EventSystem.h"

SYSTEM(SceneSystem)
	static std::vector<SmartScene> scenes = std::vector<SmartScene>();
	static SmartScene currentScene;

	//--
	//-- Creation
	//--
	template <typename T>
	T emplace(SmartScene scene, entt::entity entt) {
		auto returned = scene->registry->emplace<T>(entt);
		auto meta = entt::meta_any(returned);
		guiMeta gm{ &returned, meta.type().type_id(), entt };
		scene->guiMetas.push_back(gm);
		return returned;
	}

	template <typename T, typename ...Args>
	T emplace(SmartScene scene, entt::entity entt, Args &&...args) {
		auto returned = scene->registry->emplace<T>(entt, args...);
		auto meta = entt::meta_any(returned);
		guiMeta gm{ &returned, meta.type().type_id(), entt };
		scene->guiMetas.push_back(gm);
		return returned;
	}

	entt::entity addCoords(SmartScene scene) {
		/**/
		using namespace Systems;
		auto eCoords = scene->registry->create();
		Vertex v0{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vx{ glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vy{ glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vz{ glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) };
		Vertex vnx{ glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vny{ glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vnz{ glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) };
		Mesh coords = Mesh({ v0, vx, vy, vz, vnx, vny, vnz }, { 0,1,0, 0,2,0, 0,3,0, 0,4,0, 0,5,0, 0,6,0 }, {});
		emplace<Mesh>(scene, eCoords, coords);
		return eCoords;
		/**/
	}

	//--
	//-- Main Loop
	//--
	void update() {
		SmartScene& scene = currentScene;
		//-- Time
		scene->currentTime = glfwGetTime();
		scene->deltaTime = scene->currentTime - scene->lastTime;
		scene->lastTime = scene->currentTime;

		SmartEVW evw = EventSystem::currentEVW;

		//-- Update Events
		SceneStepEvent sev;
		sev.id = scene->id;
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

		SceneInputEvent ev;
		ev.registry = currentScene->registry;
		ev.deltaTime = currentScene->deltaTime;
		ev.key = wev.key;
		ev.scancode = wev.scancode;
		ev.action = wev.action;
		ev.mods = wev.mods;
		EventSystem::currentEVW->dispatcher.trigger<SceneInputEvent>(ev);
	}

	SmartScene create(SmartEVW evw, std::string name = "NEW SCENE") {
		// Initialize
		SmartScene scene = std::make_shared<Scene>();
		scene->registry = std::make_shared<entt::registry>();
		scene->id = rand();
		scene->name = name;

		// Required entities
		auto eRenderer = scene->registry->create();
		emplace<Renderer>(scene, eRenderer, RenderSystem::create());

		auto eCam = scene->registry->create();
		emplace<Camera>(scene,eCam);

		auto eInput = scene->registry->create();
		auto input = emplace<Input>(scene, eInput);

		addCoords(scene);

		//Add to scenes
		scenes.push_back(scene);

		//Ensure scene registry isn't empty
		if (currentScene.use_count() == 0) {
			ActivateEvent<SmartScene> ev;
			ev.member = scene;
			evw->dispatcher.trigger(ev);
		}

		ChangeEvent<SmartScene> ev;
		ev.member = scene;
		evw->dispatcher.trigger(ev);

		return scene;
	}

	void closeScene(const DestroyEvent<SmartScene>& ev) {
		for (auto& scene : scenes) {
			if (scene->id == ev.member->id) {
				scene->close(scene);
				break;
			}
		}
		DeactivateEvent<SmartScene> dev;
		dev.member = currentScene;
		EventSystem::currentEVW->dispatcher.trigger<DeactivateEvent<SmartScene>>(dev);
		currentScene = nullptr;
	}
	void openScene(const CreateEvent<SmartScene>& ev) {
		if (currentScene->id != -1)
			currentScene->close(currentScene);

		for (auto& scene : scenes) {
			if (scene->id == ev.member->id) {
				scene->open(scene);
				currentScene = scene;
				break;
			}
		}
	}

	void activateScene(ActivateEvent<SmartScene>& ev) {
		DeactivateEvent<SmartScene> dev;
		ChangeEvent<SmartScene> cev;

		dev.member = ev.member;
		ev.member->open(ev.member);
		cev.member = currentScene;
		currentScene = ev.member;

		EventSystem::currentEVW->dispatcher.trigger<DeactivateEvent<SmartScene>>(dev);
		EventSystem::currentEVW->dispatcher.trigger<ChangeEvent<SmartScene>>(cev);

	}
	void deactivateScene(DeactivateEvent<SmartScene>& ev) {
		ev.member->close(ev.member);
		//somehow tell all the systems to stop.
		//probably they should handle this independently.
	}

	//\TODO: Start implementing editor tools
	void parseGuiState(std::map<std::string, bool>& map) {

	}

	void init(SmartEVW evw) {
		// Connect event listeners
		evw->dispatcher.sink<StepEvent>().connect<update>();
		evw->dispatcher.sink<WindowInputEvent>().connect<handleInput>();
		evw->dispatcher.sink<SceneRegistryQuery>().connect<sceneRegistryQuery>();

		evw->dispatcher.sink<DestroyEvent<SmartScene>>().connect<closeScene>();
		evw->dispatcher.sink<CreateEvent<SmartScene>>().connect<openScene>();
		evw->dispatcher.sink<ActivateEvent<SmartScene>>().connect<activateScene>();
		evw->dispatcher.sink<DeactivateEvent<SmartScene>>().connect<deactivateScene>();
		//Default scene
		create(evw);
	}
END_SYSTEM

#endif
