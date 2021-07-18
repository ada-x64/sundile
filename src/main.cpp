//--
//-- main.cpp
//--
#include "./sundile/sundile.h"

//\TODO: Move these to their own files to be loaded.
void scene1(sundile::SmartEVW& evw) {
	using namespace sundile;
	using namespace sundile::Systems;

	SmartScene scene = SceneSystem::create(evw, "Suzannes");
	auto& registry = scene->registry;
	scene->open = [&](SmartScene& scene) {
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
			emplace<Model>(scene, eMonkey, suzanne);
			Model& model = registry->get<Model>(eMonkey);
			model.transform = glm::translate(model.transform, glm::vec3(10 * cos(i * 2 * pi / count), 0.f, 10 * sin(i * 2 * pi / count)));
			model.shaderCallback = [](Model m, Shader s) {
				ShaderSystem::setVec3(s, "objectColor", { 1.f,.5f,.5f });
				ShaderSystem::setVec3(s, "lightColor", { 1.f,1.f,1.f });
			};
		}

		//--
		//-- Light of our lives
		auto eLightMonkey = registry->create();
		emplace<Model>(scene, eLightMonkey, suzanne);
		Model& model = registry->get<Model>(eLightMonkey);

		//Move to LoadAssets
		Shader lightsource = ShaderSystem::create(asset_directory + "shaders/passthrough.vert", asset_directory + "shaders/light_global.frag");
		model.shader = lightsource;
		model.transform = glm::translate(model.transform, glm::vec3(0.f, 0.f, 0.f));
		model.shaderCallback = [&](Model m, Shader s) {
			ShaderSystem::setVec4(s, "color", { 1.f,1.f,1.f,1.f });
		};
	};

	evw->dispatcher.trigger<InitEvent<SmartScene>>({ scene });
}

int main(void)
{
	using namespace sundile;

	SmartEVW evw = sundile::init(); //possible override here so we can open a project file

	scene1(evw);

	sundile::mainLoop(evw);

	return 0;
}