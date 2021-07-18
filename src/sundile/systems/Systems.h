//components
#include "../globals/Common.h"
#include "./EventSystem/EventTypes.h"
#include "./InputSystem/InputTypes.h"
#include "./GuiSystem/GuiTypes.h"
#include "./RenderSystem/RenderTypes.h"
#include "./WindowSystem/WindowTypes.h"
#include "./SceneSystem/SceneTypes.h"
#include "./ProjectSystem/ProjectTypes.h"

//systems 
#include "./EventSystem/EventSystem.h"
#include "./GuiSystem/GuiMeta.h"
#include "./InputSystem/InputSystem.h"
#include "./WindowSystem/WindowSystem.h"
#include "./RenderSystem/RenderSystem.h"
#include "./SceneSystem/SceneSystem.h"
#include "./ProjectSystem/ProjectSystem.h"
#include "./GuiSystem/GuiSystem.h"


namespace sundile { namespace Systems {

	//Open a project
	void init(SmartEVW evw, SmartProject project) {
		system(("mkdir \"" + DEFAULT_PROJECT_DIRECTORY + "\"").c_str());

		GuiSystem::init(evw);
		evw->dispatcher.sink<InitEvent<SmartEVW>>().connect<CameraSystem::init>();
		evw->dispatcher.sink<InitEvent<SmartEVW>>().connect<InputSystem::init>();
		evw->dispatcher.sink<InitEvent<SmartEVW>>().connect<RenderSystem::init>();
	};
}}