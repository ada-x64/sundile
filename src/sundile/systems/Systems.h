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
		evw->dispatcher.sink<WindowInitEvent>().connect<InputSystem::windowInit>();

		evw->dispatcher.sink<SceneInitEvent>().connect<CameraSystem::init>();
		evw->dispatcher.sink<SceneInitEvent>().connect<InputSystem::init>();
		evw->dispatcher.sink<SceneInitEvent>().connect<RenderSystem::init>();

		EventSystem::initAll();
	};
}}