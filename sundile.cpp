//Implementation for Sundile.h
#include "Sundile.h"

//COMPONENT is a pure virtual struct

//ENTITY is a pure virtual class

unsigned int Entity::count = 0;

#ifdef SCENE_H
#ifndef SCENE
#define SCENE

//Constructor, deconstructor.
Scene::Scene(std::string name) {
	index = count;
	if (name == "")
		this->name = "scn_" + std::to_string(index);
	else this->name = name;
	count++;
};
Scene::~Scene() {
};

void Scene::step() {
}

void Scene::render() {
}

int Scene::count = 0;

#endif
#endif

#ifdef GAME_H
#ifndef GAME
namespace Game {
	//Anything not declared in header effectively becomes private - Linker won't know about it
	//GamePrivate namespace is not a necessary thing, but a useful syntax tool. Ensures separation.
	namespace GamePrivate {};
	using namespace GamePrivate;

	/*********************************
				 MEMBERS
	*********************************/
	namespace GamePrivate {

	}

	/*********************************
		     SCENE CONTROLS
	*********************************/
	namespace GamePrivate {
		Scene* scn_current = nullptr; //Current scene.
		std::list<Scene*>* Scenes = new std::list<Scene*>; //List of all scenes known to Game
	}

	Scene* CreateScene(std::string name) /*Creates a new scene.*/ {
		Scene* r = new Scene(name);
		Scenes->push_back(r);
		if (scn_current == nullptr)
			scn_current = r;
		return r;
	}
	void SetScene(Scene* scn) /*Sets the current scene.*/ {
		scn_current = scn;
	}
	Scene* GetScene(std::string name) {
		for (Scene* s : *Scenes) {
			if (s->name == name) {
				return s;
			}
		}
		std::cout << "Warning: Game::GetScene could not find scene with name '" << name << "'\n";
		return nullptr;
	}
	void DestroyScene(std::string name) /* Removes Scene from Game by name */ {
		DestroyScene(GetScene(name));
	}
	void DestroyScene(Scene* scn) /* Removes Scene from Game by reference */ {
		Scenes->remove(scn);
		delete scn;
	}

	/*********************************
		        MAIN LOOP
	*********************************/

	void Run(int loopcount) {
		LS ls = ls_run;
		int loops = 0; //For unit testing
		while (ls != ls_quit) {
			ls = Step();
			ls = Render();
			if (loopcount > 0) {
				loops++;
				if (loops > loopcount)
					ls = ls_quit;
			}
		}
	}
	LS Step() {
		if (scn_current != nullptr)
			scn_current->step();

		return ls_run;
	}
	LS Render() {
		if (scn_current != nullptr)
			scn_current->render();

		return ls_run;
	}

	void Exit() {
	}

}
#define GAME
#endif
#endif