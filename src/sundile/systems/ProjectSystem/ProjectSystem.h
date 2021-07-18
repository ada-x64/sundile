#ifndef SUNDILE_PROJECT_SYSTEM
#define SUNDILE_PROJECT_SYSTEM

#include "../EventSystem/EventSystem.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

SYSTEM(ProjectSystem)

	const Project emptyProject;
	SmartProject currentProject; //singlet

	template<typename Archive, typename SnapshotHandler>
	void handleArchive() {
		std::fstream fstream;
		fstream.open(currentProject->projectRoot.string() + SceneSystem::currentScene->name + ".sunScene", std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);

		//Loop through all scenes and get their registries.
		{
			Archive archive{ fstream };
			archive(SUNDILE_VERSION_NUMBER);
			archive(*(currentProject.get()));
			SnapshotHandler{ *(SceneSystem::currentScene->registry.get()) }.entities(archive);
		}

		fstream.close();
	}

	void saveProject() {
		currentProject->evw->dispatcher.trigger<SerializeEvent<SmartProject>>();
		/**/
		handleArchive<cereal::BinaryOutputArchive, entt::snapshot>();
		/**/
	}

	void loadProject(fs::path fullPath) {
		//Clear out the current project
		Project loadedProject(currentProject->evw);
		currentProject.reset(&loadedProject);
		currentProject->evw->dispatcher.trigger<DeserializeEvent<SmartProject>>();
		/**/
		handleArchive<cereal::BinaryInputArchive, entt::snapshot_loader>();
		/**/
	}

	SmartProject create(SmartEVW evw = {}, std::string name = "NEW PROJECT", fs::path projectRoot = DEFAULT_PROJECT_DIRECTORY) {
		Project* proj = new Project(evw, name, projectRoot);
		currentProject.reset(proj);
		return currentProject;
	}

END_SYSTEM

#endif