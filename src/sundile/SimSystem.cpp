//--
//-- SimSystem.cpp
//--
#include "SimSystem.h"
namespace sundile {
	namespace SimSystem {
		using namespace Components;

		namespace /* Private */ {

			//Todo: Do this properly.
			//template <typename component, typename ...dependencies>
			//void setDependencies(dependencies... args) {
			//
			//	registry->view<component>().each([=](auto entity, auto& comp) {
			//		for (auto arg : args) {
			//			if (!registry->has<typeof(arg)>(entity)) {
			//				registry->emplace<typeof(arg)>(entity);
			//			}
			//		}
			//	});
			//}
			//
			//template <typename component, typename ...dependencies>
			//void connectDependencies() {
			//	registry->on_construct<component>().connect(&setDependencies<...dependencies>);
			//}


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
		}

		namespace /*event catchers*/ {
			void catchDrawEvent(const DrawEvent& event) {

				for (SmartSim sim : sims) {
					RenderSystem::render(sim->renderer);
					//do gui
				}
			}
		}

		void windowTestEvent(const WindowEvent& event) {
			printf("calling windowTestEvent");
		}

		//--
		//-- Initialization
		//-- 
		SmartSim init(SmartEVW evw) {
			// Initialize
			SmartSim sim = std::make_shared<Sim>();
			sim->registry = std::make_shared<entt::registry>();
			sim->evw = evw;
			sim->renderer = RenderSystem::init(sim->evw, sim->registry);

			// Connect event listeners
			evw->dispatcher.sink<stepEvent>().connect<updateAll>();
			evw->dispatcher.sink<WindowInputEvent>().connect<handleInput>();
			evw->dispatcher.sink<DrawEvent>().connect<catchDrawEvent>();

			evw->dispatcher.trigger<SimInitEvent>(SimInitEvent{EventType::generic_game, sim->registry, 0.f, sim->evw});

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
			evw->dispatcher.update<DrawEvent>();
			evw->dispatcher.trigger<SimStepEvent>(SimStepEvent {EventType::generic_game, sim->registry });
		}

		void updateAll() {
			for (auto sim : sims) {
				update(sim);
			}
		}

	}
}
