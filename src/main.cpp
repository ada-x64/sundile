//--
//-- main.cpp
//--
#include "components/AllComponents.h"
#include "systems/AllSystems.h"

#include <filesystem>
//Filesystem test
void listCWD(std::filesystem::path path, bool recursive = false) {
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		std::cout << entry.path() << std::endl;
		if (recursive && entry.is_directory()) {
			listCWD(entry.path(), true);
		}
	}
}

int main(void)
{
	using namespace sundile;

	//listCWD("./", true);

	//Initialize
	SmartEVW	evw			= EventSystem::create();
	WindowSystem::initGLFW();
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	SmartWindow winc		= WindowSystem::initWindowedFullscreen(evw);
	winc->name = "sundile";
	glfwSetWindowTitle(winc->window.get(), winc->name);
	glfwSetWindowSizeLimits(winc->window.get(), winc->WIDTH, winc->HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
	SmartSim	sim			= SimSystem::init(evw);


	Systems::init(evw);
	Systems::GuiSystem::init(winc->window.get(), "#version 130"); //blehhhhh
	Systems::GuiSystem::registerECS(winc, sim);
	EventSystem::initAll();

	//Populate registry - i.e., load scene
	{
		//Prelim
		using namespace Components;
		using namespace Systems;
		auto registry = sim->registry;

		//Assets
		Model suzanne = ModelSystem::loadModel("./assets/models/monkey.obj");

		//Renderer
		auto eRenderer = registry->create();
		emplace<Renderer>(registry,eRenderer);

		//--
		//-- Camera
		auto eCam = registry->create();
		emplace<camera>(registry,eCam);
		emplace<input>(registry,eCam);

		//--
		//-- Suzannes in a Circle
		int count = 8;
		for (int i = 0; i < count; i++) {
			auto eMonkey = registry->create();
			auto model = emplace<Model>(registry, eMonkey, suzanne);
			emplace<visible>(registry, eMonkey);
			position p;
			p.pos = glm::vec3(10 * cos(i * 2 * glm::pi<float>() / count), 0.f, 10 * sin(i * 2 * glm::pi<float>() / count));
			emplace<position>(registry, eMonkey, p);
		}

		//--
		//-- Light of our lives
		auto eLightMonkey = registry->create();
		auto model = emplace<Model>(registry,eLightMonkey, suzanne);
		emplace<visible>(registry,eLightMonkey);
		position p; p.pos = { 0,0,0 };
		emplace<position>(registry, eLightMonkey, p);
		Shader lightsource = ShaderSystem::init("./assets/shaders/passthrough.vert", "./assets/shaders/light_global.frag");
		ShaderSystem::use(lightsource);
		ShaderSystem::setVec4(lightsource, "color", { 1.f,1.f,1.f,1.f });
		registry->emplace<Shader>(eLightMonkey, lightsource);

		//--
		//-- Coord map
		/**
		auto eCoords = registry->create();
		Vertex v0{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vx{ glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vy{ glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vz{ glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) };
		Vertex vnx{ glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vny{ glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vnz{ glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) };
		Mesh coords = Mesh({ v0, vx, vy, vz, vnx, vny, vnz }, { 0,1,0, 0,2,0, 0,3,0, 0,4,0, 0,5,0, 0,6,0 }, {});
		registry->emplace<Mesh>(eCoords, coords);
		emplace<wireframe>(registry,eCoords);
		emplace<visible>(registry,eCoords);
		registry->emplace<position>(eCoords, glm::vec3(0.f, 0.f, 0.f));
		/**/
	}


	//main loop

	while (EventSystem::run) {
		EventSystem::updateAll();
	}

	EventSystem::terminateAll();

	return 0;
}