//--
//-- main.cpp
//--
#include "components/AllComponents.h"
#include "systems/AllSystems.h"

int main(void)
{
	using namespace sundile;
	//When ProjectSystem is implemented, will need to set a project root directory.
	//For now, just ensure that you're executing the program from the same place it's stored :)

	//Initialize
	SmartEVW evw = EventSystem::create();
	SmartSim sim = SimSystem::init(evw);
	SmartWindow winc = WindowSystem::initWindowedFullscreen(evw);
	winc->title = "sundile";
	winc->guiEnabled = true;
	glfwSetWindowTitle(winc->window.get(), winc->title);
	glfwSetWindowSizeLimits(winc->window.get(), winc->WIDTH, winc->HEIGHT, winc->WIDTH, winc->HEIGHT);

	GuiSystem::init(evw);
	Systems::init(evw);

	//Scene registration
	{
		//Prelim
		using namespace Components;
		using namespace Systems;
		auto registry = sim->registry;

		//Assets
		Model suzanne = ModelSystem::loadModel("./assets/models/monkey.obj");

		auto eRenderer = registry->create();
		emplace<Renderer>(registry, eRenderer,RenderSystem::create());

		auto eCam = registry->create();
		emplace<camera>(registry, eCam);

		//--
		//-- Suzannes in a Circle
		int count = 8;
		for (int i = 0; i < count; i++) {
			auto eMonkey = registry->create();
			auto model = emplace<Model>(registry,  eMonkey, suzanne);
			emplace<visible>(registry,  eMonkey);
			position p;
			p.pos = glm::vec3(10 * cos(i * 2 * glm::pi<float>() / count), 0.f, 10 * sin(i * 2 * glm::pi<float>() / count));
			emplace<position>(registry,  eMonkey, p);
		}

		//--
		//-- Light of our lives
		auto eLightMonkey = registry->create();
		auto model = emplace<Model>(registry, eLightMonkey, suzanne);
		emplace<visible>(registry, eLightMonkey);
		position p; p.pos = { 0,0,0 };
		emplace<position>(registry,  eLightMonkey, p);
		Shader lightsource = ShaderSystem::init("./assets/shaders/passthrough.vert", "./assets/shaders/light_global.frag");
		ShaderSystem::use(lightsource);
		ShaderSystem::setVec4(lightsource, "color", { 1.f,1.f,1.f,1.f });
		emplace<Shader>(registry, eLightMonkey, lightsource);

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
		emplace<Mesh>(registry, eCoords, coords);
		emplace<wireframe>(registry, eCoords);
		emplace<visible>(registry, eCoords);
		emplace<position>(registry, eCoords, glm::vec3(0.f, 0.f, 0.f));
		/**/
	}

	//main loop
	while (EventSystem::run) {
		EventSystem::updateAll();
	}

	EventSystem::terminateAll();

	return 0;
}