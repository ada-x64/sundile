//--
//-- main.cpp
//--
#include "sundile/sundile.h"
#include "systems/AllSystems.h"

int main(void)
{
	using namespace sundile;

	//Initialize
	SmartEVW	evw			= EventSystem::create();
	SmartWindow winc		= WindowSystem::init(evw, "Rendering window");
	//SmartWindow renderInfo	= WindowSystem::init(evw, 150, 600, "Render Info");
	SmartSim	sim			= SimSystem::init(evw);
	Systems::init(evw);
	EventSystem::initAll();

	//Populate registry - i.e., load scene
	{
		using namespace Components;
		auto registry = sim->registry;

		auto eCam = registry->create();
		registry->emplace<camera>(eCam);
		registry->emplace<input>(eCam);

		int count = 8;
		for (int i = 0; i < count; i++) {
			auto eMonkey = registry->create();
			registry->emplace<Model>(eMonkey, "./assets/models/monkey.obj");
			registry->emplace<visible>(eMonkey);
			registry->emplace<position>(eMonkey, glm::vec3(10*cos(i * 2*glm::pi<float>()/ count), 0.f, 10*sin(i * 2*glm::pi<float>()/ count)));
		}

		auto eLightMonkey = registry->create();
		registry->emplace<Model>(eLightMonkey, "./assets/models/monkey.obj");
		registry->emplace<visible>(eLightMonkey);
		registry->emplace<position>(eLightMonkey, glm::vec3(0.f, 0.f, 0.f));
		Shader lightsource = ShaderSystem::init("./assets/shaders/passthrough.vert", "./assets/shaders/light.frag");
		ShaderSystem::use(lightsource);
		ShaderSystem::setVec4(lightsource, "color", { 1.f,1.f,1.f,1.f });
		registry->emplace<Shader>(eLightMonkey, lightsource);

		//auto eCoords = registry->create();
		//Vertex v0{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		//Vertex vx{ glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		//Vertex vy{ glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		//Vertex vz{ glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) };
		//Vertex vnx{ glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		//Vertex vny{ glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		//Vertex vnz{ glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) };
		//Mesh coords = Mesh({ v0, vx, vy, vz, vnx, vny, vnz }, { 0,1,0, 0,2,0, 0,3,0, 0,4,0, 0,5,0, 0,6,0 }, {});
		//registry->emplace<Mesh>(eCoords, coords);
		//registry->emplace<wireframe>(eCoords);
		//registry->emplace<visible>(eCoords);
		//registry->emplace<position>(eCoords, glm::vec3(0.f, 0.f, 0.f));

		/**
		auto eTree = registry->create();
		registry->emplace<Model>(eTree, "./assets/models/Trees/OakTree1.fbx");
		registry->emplace<visible>(eTree);
		registry->emplace<position>(eTree, glm::vec3(10.f, 10.f, 0.f));
		registry->emplace<Shader>(eTree, ShaderSystem::init("./assets/shaders/passthrough.vert", "./assets/shaders/tex_diffuse1.frag"));
		/**/
	}


	//main loop

	while (EventSystem::run) {
		EventSystem::updateAll();
	}

	EventSystem::terminateAll();

	return 0;
}