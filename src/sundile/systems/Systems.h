//components
//- TODO: Migrate all types into the appropriate file
#include "../globals/Common.h"
#include "./EventSystem/EventTypes.h"
#include "./GuiSystem/GuiTypes.h"
#include "./GuiSystem/GuiMeta.h"
#include "./SceneSystem/SceneTypes.h"
#include "./InputSystem/InputTypes.h"
#include "./ProjectSystem/ProjectTypes.h" //todo
#include "./RenderSystem/RenderTypes.h" //todo
#include "./WindowSystem/WindowTypes.h"

//systems 
#include "./EventSystem/EventSystem.h"
#include "./WindowSystem/WindowSystem.h"
#include "./RenderSystem/RenderSystem.h"
#include "./SceneSystem/SceneSystem.h"
#include "./ProjectSystem/ProjectSystem.h"
#include "./GuiSystem/GuiSystem.h"
#include "./InputSystem/InputSystem.h"

namespace sundile { namespace Systems {

	//Open a directory that it might be made into a project root folder
	void init(SmartEVW evw, fs::path projectRoot) {
		//Project proj; //(projectRoot);
		//init(evw, proj);
	};

	//Open a project
	void init(SmartEVW evw, Project& project) {

		ProjectSystem::currentProject.reset(&project);

		//Load program settings
		// read in serialized file with data for sundile


		//Load project
		// read in serialized file with data that points to project directory, etc
		system(("mkdir \"" + DEFAULT_PROJECT_DIRECTORY + "\"").c_str());
		//ProjectSystem::loadProject();

		//Sink initialization events

		//TODO: 
		/*
		* template<typename T>
		* sinkEvents {
			evw->dispathcer.sink<InitEvent>().connect<T::init>();
			}
		*/

		evw->dispatcher.sink<InitEvent>().connect<CameraSystem::init>();
		evw->dispatcher.sink<InitEvent>().connect<InputSystem::init>();
		evw->dispatcher.sink<InitEvent>().connect<RenderSystem::init>();
	};
}}