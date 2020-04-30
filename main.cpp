//--
//-- main.cpp
//--
#include "sundile.h"

int main(void)
{
	using namespace sundile;
	//Define and initialize renderer and GLFW wrapper
	Renderer::init();

	//Set up nanogui
	nanogui::Screen *screen = new nanogui::Screen();
	screen->initialize(Window::window, true);

	//lambda to populate registry.
	[]()->void {
		using namespace Systems;
		using namespace Components;

		auto eCam = registry.create();
		registry.assign<camera>(eCam);

		auto eMonkey = registry.create();
		registry.assign<Model>(eMonkey, "./models/monkey.obj");
		registry.assign<visible>(eMonkey);
		registry.assign<position>(eMonkey, glm::vec3(0.f, 0.f, 0.f));

		auto eTree = registry.create();
		registry.assign<Model>(eTree, "./assets/Trees/OakTree1.fbx");
		registry.assign<visible>(eTree);
		registry.assign<position>(eTree, glm::vec3(10.f, 10.f, 0.f));
		registry.assign<Shader>(eTree, "./shaders/passthrough.vert", "./shaders/tex_diffuse1.frag");

		//TODO - Move this
		setDependencies();
		connectListeners();
	};


	//Loop until the user closes the window
	while (!glfwWindowShouldClose(Window::window))
	{
		//Update
		Systems::step();
		Renderer::render();
		Window::update();
	}

	glfwTerminate();
	return 0;
}