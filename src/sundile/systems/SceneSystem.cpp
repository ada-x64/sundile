//--
//-- SceneSystem.cpp
//--
#include "SceneSystem.h"
namespace sundile {
	namespace SceneSystem {

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

		void catchInit(const initEvent& ev) {
			for (auto scene : scenes) {
				SceneInitEvent ev;
				ev.registry = scene->registry;
				ev.deltaTime = 0.f;
				ev.evw = scene->evw;
				ev.id = scene->id;
				ev.evw->dispatcher.trigger<SceneInitEvent>(ev);
			}
		}

		SmartScene init(SmartEVW evw) {
			// Initialize
			SmartScene scene = std::make_shared<Scene>();
			scene->registry = std::make_shared<entt::registry>();
			scene->evw = evw;
			scene->id = rand();

			// Connect event listeners
			evw->dispatcher.sink<stepEvent>().connect<updateAll>();
			evw->dispatcher.sink<WindowInputEvent>().connect<handleInput>();
			evw->dispatcher.sink<initEvent>().connect<catchInit>();
			evw->dispatcher.sink<SceneRegistryQuery>().connect<sceneRegistryQuery>();


			//Add to scenes
			scenes.push_back(scene);

			return scene;
		}

		//--
		//-- Main Loop
		//--
		void update(SmartScene scene) {

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

		void updateAll() {
			for (auto scene : scenes) {
				update(scene);
			}
		}

	}
}
