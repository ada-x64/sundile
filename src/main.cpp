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

ImVec2 getWindowSize(SmartWindow winc) {
	int wwidth = 0, wheight = 0;
	glfwGetWindowSize(winc->window.get(), &wwidth, &wheight);
	if (wwidth && wheight) {
		winc->HEIGHT = wwidth;
		winc->WIDTH = wheight;
	}
	float width = static_cast<float>(winc->WIDTH);
	float height = static_cast<float>(winc->HEIGHT);
	return ImVec2{ width, height };

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

		//--
		//-- GUI
		float ww = winc->WIDTH;
		float wh = winc->HEIGHT;
		float viewport_w = 800;
		float viewport_h = 600;
		float viewport_x = ww / 2 - viewport_w/2;
		float viewport_y = wh / 2 - viewport_h/2;
		glViewport(viewport_x, viewport_y, viewport_w, viewport_h);

		

		auto renderWindow = registry->create();
		registry->emplace<guiElement>(renderWindow, [=]() {
			using namespace ImGui;
			SetNextWindowBgAlpha(0.f);
			SetNextWindowSizeConstraints({ viewport_w, viewport_h }, { viewport_w, viewport_h });
			Begin("rendering frame");
			auto pos = ImGui::GetWindowPos();
			SetWindowSize({ 800, 600 });
			glViewport(pos.x, -pos.y+101 , viewport_w, viewport_h); //magic number here - don't know what the vertical offset between imgui and gl is. i guess it's 101.
			End();

			GuiEvent e; //TODO: make this better
			e.content = GuiEventContent{ ImGui::IsWindowFocused() };
			sim->evw->dispatcher.trigger<GuiEvent>(e);
		});

		auto inspector = registry->create();
		ImVec2 inspectorSize = { 240, static_cast<float>(winc->WIDTH) };
		registry->emplace<guiElement>(inspector, [=]() {

			ImVec2 windowSize = getWindowSize(winc);

			using namespace ImGui;
			SetNextWindowSize(inspectorSize);
			SetNextWindowSizeConstraints({ inspectorSize.x, 120 }, { inspectorSize.x, inspectorSize.y });
			//SetNextWindowPos({ windowSize.x - inspectorSize.x, 0 });

			Begin("Inspector");
			//dynamic content here
			End();
			});

		auto toolbar = registry->create();
		ImVec2 toolbarSize = { 120, static_cast<float>(winc->WIDTH) };
		registry->emplace<guiElement>(toolbar, [=]() {

			ImVec2 windowSize = getWindowSize(winc);

			using namespace ImGui;
			SetNextWindowSize(toolbarSize);
			SetNextWindowSizeConstraints({ toolbarSize.x, 32 }, { windowSize.y, toolbarSize.y });
			SetNextWindowPos({ windowSize.x - toolbarSize.x - inspectorSize.x, 0 }); //this should lock the toolbar to the right. it isn't doing that x(
			SetWindowPos("Toolbar", { windowSize.x - toolbarSize.x - inspectorSize.x, 0 });

			Begin("Toolbar");
			Text("Entity tools:");
			Button("Add");
			Button("Select");
			Button("Translate");
			Button("Rotate");
			End();
		});

		//--
		//-- Camera
		auto eCam = registry->create();
		registry->emplace<camera>(eCam);
		registry->emplace<input>(eCam);

		//--
		//-- Suzannes in a Circle
		int count = 8;
		for (int i = 0; i < count; i++) {
			auto eMonkey = registry->create();
			auto& model = registry->emplace<Model>(eMonkey);
			model = suzanne;
			registry->emplace<visible>(eMonkey);
			registry->emplace<position>(eMonkey, glm::vec3(10*cos(i * 2*glm::pi<float>()/ count), 0.f, 10*sin(i * 2*glm::pi<float>()/ count)));
		}

		//--
		//-- Light of our lives
		auto eLightMonkey = registry->create();
		auto& model = registry->emplace<Model>(eLightMonkey);
		model = suzanne;
		registry->emplace<visible>(eLightMonkey);
		registry->emplace<position>(eLightMonkey, glm::vec3(0.f, 0.f, 0.f));
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
		registry->emplace<wireframe>(eCoords);
		registry->emplace<visible>(eCoords);
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