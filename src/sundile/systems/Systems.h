#include "./EventSystem/EventSystem.h"
#include "./WindowSystem.h"
#include "./RenderSystem/RenderSystem.h"
#include "./SceneSystem/SceneSystem.h"
#include "./GuiSystem/GuiSystem.h"
#include "./InputSystem.h"
#include "./ProjectSystem/ProjectSystem.h"

namespace sundile { namespace Systems {
	
	static Project currentProject = ProjectSystem::emptyProject;

	//Open a directory that it might be made into a project root folder
	void init(SmartEVW evw, fs::path projectRoot) {
		init(evw, { projectRoot });
	};

	//Open a project
	void init(SmartEVW evw, Project project = ProjectSystem::emptyProject) {
		//do something!!
	};
}}