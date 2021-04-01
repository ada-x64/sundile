#ifndef SUNDILE_PROJECT_SYSTEM
#define SUNDILE_PROJECT_SYSTEM

#include "../EventSystem/EventSystem.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

COMPONENT(Project)
	//DATA
	std::string name;
	SmartEVW evw;
	fs::path projectRoot;

	Project(Project& other)
		: evw(other.evw), name(other.name), projectRoot(other.projectRoot) {}
	Project(SmartEVW evw = {}, std::string name = "NEW PROJECT", fs::path projectRoot = DEFAULT_PROJECT_DIRECTORY)
		: projectRoot(projectRoot), name(name), evw(evw) {}

	SERIALIZE(Project, name, projectRoot.string()); //\TODO: this isn't working
END_COMPONENT

namespace sundile {
	typedef std::unique_ptr<Project> SmartProject;
}

SYSTEM(ProjectSystem)

	const Project emptyProject;
	SmartProject currentProject; //singlet

	template<typename Archive, typename SnapshotHandler>
	void handleArchive() {
		std::fstream fstream;
		fstream.open(currentProject->projectRoot.string() + currentProject->name + PROJECT_EXTENSION, std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);

		//Loop through all scenes and get their registries.
		{
			Archive archive{ fstream };
			archive(*(currentProject.get()));
			//archive(SUNDILE_VERSION_NUMBER);
			for (auto it = SceneSystem::scenes.begin(); it != SceneSystem::scenes.end(); ++it) {
				auto scene = *it;
				SnapshotHandler{ *(scene->registry.get()) }.entities(archive);
			}
		}
	}

	void saveProject() {
		currentProject->evw->dispatcher.trigger<ProjSaveEvent>();
		handleArchive<cereal::BinaryOutputArchive, entt::snapshot>();
	}

	void loadProject(fs::path fullPath) {
		//Clear out the current project
		Project loadedProject(currentProject->evw);
		currentProject.reset(&loadedProject);
		currentProject->evw->dispatcher.trigger<ProjLoadEvent>();
		handleArchive<cereal::BinaryInputArchive, entt::snapshot_loader>();
	}
END_SYSTEM

#endif