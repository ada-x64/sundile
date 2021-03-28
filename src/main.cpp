//--
//-- main.cpp
//--
#include "./sundile/sundile.h"

//\TODO: Move these to their own files to be loaded.
void scene1(sundile::SmartEVW evw) {
	using namespace sundile;

	SmartScene scene1 = Systems::SceneSystem::create(evw);
	auto& registry = scene1->registry;
	scene1->open = [](SmartRegistry& registry) {
		//Prelim
		using namespace Components;
		using namespace Systems;

		//Assets
		Model suzanne = ModelSystem::loadModel(asset_directory + "models/monkey.obj");

		//--
		//-- Suzannes in a Circle
		int count = 8;
		for (int i = 0; i < count; i++) {
			auto eMonkey = registry->create();
			Model model = emplace<Model>(registry, eMonkey, suzanne);
			registry->get<Model>(eMonkey).transform = glm::translate(model.transform, glm::vec3(10 * cos(i * 2 * pi / count), 0.f, 10 * sin(i * 2 * pi / count)));
		}

		//--
		//-- Light of our lives
		auto eLightMonkey = registry->create();
		auto model = emplace<Model>(registry, eLightMonkey, suzanne);
		Shader lightsource = ShaderSystem::create(asset_directory + "shaders/passthrough.vert", asset_directory + "shaders/light_global.frag");
		ShaderSystem::use(lightsource);
		ShaderSystem::setVec4(lightsource, "color", { 1.f,1.f,1.f,1.f });
		emplace<Shader>(registry, eLightMonkey, lightsource);

	};
}

void scene2(sundile::SmartEVW evw) {
	using namespace sundile;
	SmartScene scene2 = Systems::SceneSystem::create(evw);
	auto& registry = scene2->registry;
	scene2->open = [](SmartRegistry& registry) {
		//Boilerplate
		using namespace Components;
		using namespace Systems;

		//Assets
		Model suzanne = ModelSystem::loadModel(asset_directory + "models/monkey.obj");
		auto eSuzanne = registry->create();
		emplace<Model>(registry, eSuzanne, suzanne);

	};
}

int main(void)
{
	using namespace sundile;

	SmartEVW evw = sundile::init(); //possible override here so we can open a project file

	scene1(evw);
	scene2(evw);

	sundile::mainLoop();

	return 0;
}