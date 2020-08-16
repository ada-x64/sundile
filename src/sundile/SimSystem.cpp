//--
//-- SimSystem.cpp
//--
#include "SimSystem.h"
namespace sundile {
	namespace SimSystem {
		using namespace Components;

		//Parses GLFW key events and passes appropriate information to other functions.
		void handleInput(const WindowInputEvent& wev) {
			//-- Emergency Exit
			if (wev.key == GLFW_KEY_ESCAPE) {
				printf("Exiting on Keypress: Esc");
				exit(EXIT_FAILURE);
			}

			for (auto sim : sims) {
				sim->evw->dispatcher.trigger<SimInputEvent>(SimInputEvent{ wev.type, sim->registry, sim->deltaTime, wev.key, wev.scancode, wev.action, wev.mods });
			}
		}

		void catchInit(const initEvent& ev) {
			for (auto sim : sims) {
				ev.evw->dispatcher.trigger<SimInitEvent>(SimInitEvent{ EventType::generic_game, sim->registry, 0.f, sim->evw });
			}
		}

		SmartSim init(SmartEVW evw) {
			// Initialize
			SmartSim sim = std::make_shared<Sim>();
			sim->registry = std::make_shared<entt::registry>();
			sim->evw = evw;

			// Connect event listeners
			evw->dispatcher.sink<stepEvent>().connect<updateAll>();
			evw->dispatcher.sink<WindowInputEvent>().connect<handleInput>();
			evw->dispatcher.sink<initEvent>().connect<catchInit>();


			//Add to sims
			sims.push_back(sim);

			return sim;
		}

		//--
		//-- Main Loop
		//--
		void update(SmartSim sim) {

			//-- Time
			float currentTime = glfwGetTime();
			sim->deltaTime = currentTime - sim->lastTime;
			sim->lastTime = currentTime;

			SmartEVW evw = sim->evw;

			//-- Update Events
			SimStepEvent sev = { EventType::generic_game, sim->registry };
			RenderEvent rev = { EventType::generic_render, sim->registry, sim->deltaTime };
			GuiEvent gev = { EventType::generic_gui, sim->registry, sim->deltaTime };
			evw->dispatcher.trigger<SimStepEvent>(sev);
			evw->dispatcher.trigger<RenderEvent>(rev);
			evw->dispatcher.trigger<GuiEvent>(gev);
		}

		void updateAll() {
			for (auto sim : sims) {
				update(sim);
			}
		}

	}
}
