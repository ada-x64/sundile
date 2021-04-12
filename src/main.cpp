//--
//-- main.cpp
//--
#include "./sundile/sundile.h"

//\TODO: Move these to their own files to be loaded.
void scene1(sundile::SmartEVW& evw) {
	using namespace sundile;

	SmartScene scene = Systems::SceneSystem::create(evw, "Suzannes");
	auto& registry = scene->registry;
	scene->open = [](SmartScene& scene) {
		//Prelim
		using namespace Components;
		using namespace Systems;
		using namespace SceneSystem;

		auto registry = scene->registry;

		//Assets
		Model suzanne = ModelSystem::loadModel(asset_directory + "models/monkey.obj");

		//--
		//-- Suzannes in a Circle
		int count = 8;
		for (int i = 0; i < count; i++) {
			auto eMonkey = registry->create();
			Model model = emplace<Model>(scene, eMonkey, suzanne);
			registry->get<Model>(eMonkey).transform = glm::translate(model.transform, glm::vec3(10 * cos(i * 2 * pi / count), 0.f, 10 * sin(i * 2 * pi / count)));
		}

		//--
		//-- Light of our lives
		auto eLightMonkey = registry->create();
		auto model = emplace<Model>(scene, eLightMonkey, suzanne);
		Shader lightsource = ShaderSystem::create(asset_directory + "shaders/passthrough.vert", asset_directory + "shaders/light_global.frag");
		ShaderSystem::use(lightsource);
		ShaderSystem::setVec4(lightsource, "color", { 1.f,1.f,1.f,1.f });
		emplace<Shader>(scene, eLightMonkey, lightsource);

	};

	SceneInitEvent ev;
	ev.id = scene->id;
	ev.evw = evw;
	ev.registry = scene->registry;
	evw->dispatcher.trigger<SceneInitEvent>(ev);
}

void scene2(sundile::SmartEVW& evw) {
	using namespace sundile;
	SmartScene scene = Systems::SceneSystem::create(evw, "Suzanne");
	auto& registry = scene->registry;
	scene->open = [](SmartScene& scene) {
		//Boilerplate
		using namespace Components;
		using namespace Systems;

		//Assets
		Model suzanne = ModelSystem::loadModel(asset_directory + "models/monkey.obj");
		auto eSuzanne = scene->registry->create();
		SceneSystem::emplace<Model>(scene, eSuzanne, suzanne);

	};

	SceneInitEvent ev;
	ev.id = scene->id;
	ev.evw = evw;
	ev.registry = scene->registry;
	evw->dispatcher.trigger<SceneInitEvent>(ev);
}

int main(void)
{
	using namespace sundile;

	SmartEVW evw = sundile::init(); //possible override here so we can open a project file

	scene1(evw);
	scene2(evw);

	sundile::mainLoop(evw);

	return 0;
}