//--
//-- main.cpp
//--
#include "./headers/sundile.h"

int main(void)
{
	using namespace sundile;

	//Initialize
	SmartEVW evw = std::make_shared<EventWrapper>();
	SmartWindow winc_renderer = WindowSystem::init(evw);
	Game game = GameSystem::init(evw);
	Renderer renderer = RenderSystem::init(evw, game);

	//Populate registry
	{
		using namespace Components;

		auto eCam = game.registry.create();
		game.registry.emplace<camera>(eCam);

		auto eMonkey = game.registry.create();
		game.registry.emplace<Model>(eMonkey, "./assets/models/monkey.obj");
		game.registry.emplace<visible>(eMonkey);
		game.registry.emplace<position>(eMonkey, glm::vec3(0.f, 0.f, 0.f));

		auto eTree = game.registry.create();
		game.registry.emplace<Model>(eTree, "./assets/models/Trees/OakTree1.fbx");
		game.registry.emplace<visible>(eTree);
		game.registry.emplace<position>(eTree, glm::vec3(10.f, 10.f, 0.f));
		game.registry.emplace<Shader>(eTree, ShaderSystem::init("./assets/shaders/passthrough.vert", "./assets/shaders/tex_diffuse1.frag"));
	}


	//main loop
	while (WindowSystem::windows.size() > 0) {
		WindowSystem::updateAll();
		/**
		for (auto window : WindowSystem::windows) {
			//Update
			WindowSystem::update(window);
			GameSystem::step(game);
			RenderSystem::render(renderer);
		}
		/**/
	}


	//EnttSystem::terminate(evw);
	RenderSystem::terminate(renderer);
	// shouldn't be needed
	for (auto window : WindowSystem::windows) {
		WindowSystem::terminate(window);
	}
	return 0;
}