//--
//-- main.cpp
//--
#include "./headers/sundile.h"

int main(void)
{
	using namespace sundile;

	//Initialize
	SmartEVW	evw			= EventSystem::create();
	SmartWindow winc		= WindowSystem::init(evw, "Rendering window");
	SmartWindow renderInfo	= WindowSystem::init(evw, 150, 600, "Render Info");
	SmartSim	sim			= SimSystem::init(evw);

	//Populate registry - i.e., load scene
	{
		using namespace Components;
		auto registry = sim->registry;

		auto eCam = registry->create();
		registry->emplace<camera>(eCam);

		auto eMonkey = registry->create();
		registry->emplace<Model>(eMonkey, "./assets/models/monkey.obj");
		registry->emplace<visible>(eMonkey);
		registry->emplace<position>(eMonkey, glm::vec3(0.f, 0.f, 0.f));

		auto eTree = registry->create();
		registry->emplace<Model>(eTree, "./assets/models/Trees/OakTree1.fbx");
		registry->emplace<visible>(eTree);
		registry->emplace<position>(eTree, glm::vec3(10.f, 10.f, 0.f));
		registry->emplace<Shader>(eTree, ShaderSystem::init("./assets/shaders/passthrough.vert", "./assets/shaders/tex_diffuse1.frag"));
	}


	//main loop
	EventSystem::initAll();

	while (EventSystem::run) {
		EventSystem::updateAll();
	}

	EventSystem::terminateAll();

	return 0;
}