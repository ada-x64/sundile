#ifndef SUNDILE_PROJECT_SYSTEM
#define SUNDILE_PROJECT_SYSTEM

#include "../EventSystem/EventSystem.h"

BEGIN_COMPONENT(Project)
fs::path projectRoot;

END_COMPONENT

BEGIN_SYSTEM(ProjectSystem)
static const Project emptyProject;
	
END_SYSTEM

#endif