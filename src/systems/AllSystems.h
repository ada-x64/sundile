//-- (automatically generated by watch-headers.py)
//--
//-- EXPORT functions
//--
#include "CameraSystem.h"
#include "InputSystem.h"
#include "MeshSystem.h"
#include "ModelSystem.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "ShaderSystem.h"

namespace sundile {
	namespace Systems {
		void init(SmartEVW evw, SmartWindow winc, SmartSim sim) {
#ifndef SUNDILE_EXPORT
			Systems::GuiSystem::init(winc->window.get()); //blehhhhh
			Systems::GuiSystem::registerECS(winc, sim);
#endif
			evw->dispatcher.sink<SimInitEvent>().connect<CameraSystem::init>();
			evw->dispatcher.sink<SimInitEvent>().connect<InputSystem::init>();
			evw->dispatcher.sink<SimInitEvent>().connect<RenderSystem::init>();
			evw->dispatcher.sink<SimInitEvent>().connect<GuiSystem::simInit>();

			EventSystem::initAll();
		}
	}
}