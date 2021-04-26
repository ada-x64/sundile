//components
#include "../globals/Common.h"
#include "./EventSystem/EventTypes.h"
#include "./GuiSystem/GuiTypes.h"
#include "./GuiSystem/GuiMeta.h"
#include "./RenderSystem/RenderTypes.h"
#include "./InputSystem/InputTypes.h"
#include "./ProjectSystem/ProjectTypes.h"
#include "./WindowSystem/WindowTypes.h"
#include "./SceneSystem/SceneTypes.h"

//systems 
#include "./EventSystem/EventSystem.h"
#include "./WindowSystem/WindowSystem.h"
#include "./RenderSystem/RenderSystem.h"
#include "./SceneSystem/SceneSystem.h"
#include "./ProjectSystem/ProjectSystem.h"
#include "./GuiSystem/GuiSystem.h"
#include "./InputSystem/InputSystem.h"


namespace sundile { namespace Systems {

	//Open a project
	void init(SmartEVW evw, Project& project) {
		system(("mkdir \"" + DEFAULT_PROJECT_DIRECTORY + "\"").c_str());
		ProjectSystem::currentProject.reset(&project);

		GuiSystem::init(evw);
		evw->dispatcher.sink<InitEvent>().connect<CameraSystem::init>();
		evw->dispatcher.sink<InitEvent>().connect<InputSystem::init>();
		evw->dispatcher.sink<InitEvent>().connect<RenderSystem::init>();
	};
}}