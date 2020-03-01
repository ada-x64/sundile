
/*
==================================
   _____                 ___ __
  / ___/__  ______  ____/ (_) /__
  \__ \/ / / / __ \/ __  / / / _ \
 ___/ / /_/ / / / / /_/ / / /  __/
/____/\__,_/_/ /_/\__,_/_/_/\___/

==================================
MODABLE, SINGLE-HEADER SOLUTION
GAME ENGINE PROTOTYPE
==================================
Author: Phoenix Ada Rose Mandala
Last Edited: 2-27-2020
==================================
*/


//Headers
#pragma once
#include <string>
#include <list>
#include <algorithm>
#include <iostream>

#ifndef SUNDILE
#define SUNDILE //This is used to check if this header has been linked.

/*********************************
     ARCHITECTURE DEFINITIONS
*********************************/
class Scene;
namespace Game {};

#ifndef COMPONENT_H
#define COMPONENT_H
typedef struct Component { //Components are little packets of data. They comprise Entities. EX: c_Foo
	//Static methods and component storage
	static std::list<Component*> store;
	template<typename c>
	static c* find(unsigned int id) {
		typename std::list<c*>::iterator it = std::find_if(std::begin(c::store), std::end(c::store), [&](c component) {
			return component.id == id;
		});
		if (it == std::end(c::store)) return nullptr;
		return *it;
	}

	//Required component features
	unsigned int id;
	bool enabled;
	Component(unsigned int id, bool enabled = true) {
		this->id = id;
		this->enabled = enabled;
		store.push_front(this);
	}
};
#endif

#ifndef ENTITY_H
#define ENTITY_H
typedef struct Entity { //Entities "contain" components. They are the "things" in your game. EX: e_Foo
	static unsigned int count;
	unsigned int id;
	Entity() {
		this->id = count;
		count++;
	}

	template <typename c>
	c add() {
		c *component = new c(this->id);
		return *component;
	}

	template <typename c>
	bool remove() {
		c *component = Component::find<c>(id);
		if (component == nullptr) return false;
		c::store.remove(component);
		return true;
	}

	template <typename c>
	bool has() {
		return !(Component::find<c>(id) == nullptr);
	}

	template <typename c>
	c get() {
		return *Component::find<c>(id);
	}
};

#endif // !ENTITY_H

#ifndef SYSTEM_H
#define SYSTEM_H

class System { //Systems define interactions between Components and cause Entities to act. EX: s_Foo.

};

#endif // !SYSTEM_H

#ifndef SCENE_H
#define SCENE_H

class Scene { //Scenes contain Entities and Systems. They are little stages for your plays to be acted out. EX: scn_Foo.
private:
	//Private members.
	std::list<Entity*> Entities;
	//Static members.
	static int count;

public:
	std::string name;
	int index;
	bool active;

	Scene(std::string name = "");
	~Scene();

	static int getCount() {return count;}
	void step();
	void render();
};

#endif // !ROOM_H

#ifndef GAME_H
#define GAME_H
//The Game wraps up all the Scenes and presents them in a meaningful way. 
namespace Game {
	/*********************************
			 SCENE CONTROLS
	*********************************/
	Scene* CreateScene(std::string name = "");
	void SetScene(Scene* scn);
	Scene* GetScene(std::string name);
	void DestroyScene(std::string name);
	void DestroyScene(Scene* scn);

	/*********************************
			   MAIN LOOP
	*********************************/
	enum LS { //Loop Status
		ls_run,
		ls_quit,
		ls_error,
		ls_last
		//To be expanded as needed
	};
	void Run(int loopcount = -1);
	LS Step();
	LS Render();
	void Exit();


};
#endif // !GAME_H

#endif