#ifndef PROJECT_TYPES
#define PROJECT_TYPES

COMPONENT(Project)
	//DATA
	std::string name;
	SmartEVW evw;
	fs::path projectRoot;

	Project(Project& other)
		: evw(other.evw), name(other.name), projectRoot(other.projectRoot) {}
	Project(SmartEVW evw = {}, std::string name = "NEW PROJECT", fs::path projectRoot = DEFAULT_PROJECT_DIRECTORY)
		: projectRoot(projectRoot), name(name), evw(evw) {}

	~Project(){
	}
	

END_COMPONENT

template<typename Archive>
void serialize(Archive& ar, sundile::Components::Project proj) {
	ar(proj.name, proj.projectRoot.string());
}


namespace sundile {
	typedef std::shared_ptr<Project> SmartProject;
}

#endif