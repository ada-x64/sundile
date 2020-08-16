//--
//-- main.cpp
//--
#include "sundile/sundile.h"
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
	SmartWindow winc		= WindowSystem::init(evw, 1280, 720);
	SmartSim	sim			= SimSystem::init(evw);
	Systems::init(evw);
	Systems::GuiSystem::init(winc->window.get(), "#version 130"); //blehhhhh
	EventSystem::initAll();

	//Populate registry - i.e., load scene
	{
		//Prelim
		using namespace Components;
		auto registry = sim->registry;

		//Assets
		Model suzanne = Model("./assets/models/monkey.obj");

		//Renderer
		auto eRenderer = registry->create();
		registry->emplace<Renderer>(eRenderer);

		//GUI
		int ww = winc->WIDTH;
		int wh = winc->HEIGHT;
		int viewport_w = 800;
		int viewport_h = 600;
		int viewport_x = ww / 2 - viewport_w/2;
		int viewport_y = wh / 2 - viewport_h/2;
		glViewport(viewport_x, viewport_y, viewport_w, viewport_h);

		auto eGUI = registry->create();
		guiElement mainMenu;
		mainMenu.renderFunc = []() {
			using namespace ImGui;
			Begin("Main Menu");
			Text("hey");
			End();
		};
		registry->emplace<guiElement>(eGUI, mainMenu);

		//Camera
		auto eCam = registry->create();
		registry->emplace<camera>(eCam);
		registry->emplace<input>(eCam);

		//Suzannes in a Circle
		int count = 8;
		for (int i = 0; i < count; i++) {
			auto eMonkey = registry->create();
			auto& model = registry->emplace<Model>(eMonkey);
			model = suzanne;
			registry->emplace<visible>(eMonkey);
			registry->emplace<position>(eMonkey, glm::vec3(10*cos(i * 2*glm::pi<float>()/ count), 0.f, 10*sin(i * 2*glm::pi<float>()/ count)));
		}

		//Light of our lives
		auto eLightMonkey = registry->create();
		auto& model = registry->emplace<Model>(eLightMonkey);
		model = suzanne;
		registry->emplace<visible>(eLightMonkey);
		registry->emplace<position>(eLightMonkey, glm::vec3(0.f, 0.f, 0.f));
		Shader lightsource = ShaderSystem::init("./assets/shaders/passthrough.vert", "./assets/shaders/light_global.frag");
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