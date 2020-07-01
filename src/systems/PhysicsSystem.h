#pragma once
#include "../sundile/Common.h"
#include "../sundile/SimSystem.h"

SYSTEM_DEF_BEGIN(PhysicsSystem)

inline SmartSim sim;

void step(const stepEvent& ev) {
	sim->registry->view<position, velocity>().each([&](auto entity, auto& _pos, auto& _vel) {
		//-- Vars
		glm::vec3& vel = _vel.vel;
		glm::vec3& pos = _pos.pos;

		//-- Camera
		if (sim->registry->has<camera>(entity)) {
			//Get cam
			camera& cam = sim->registry->get<camera>(entity);
			glm::vec3& spd = cam.spd;
			vel += spd;

			//Update position
			glm::vec3 left = glm::normalize(glm::cross(cam.target, cam.up));
			auto res = left * vel;

			cam.pos.x += res.x;
			cam.pos.y += vel.y;
			cam.pos.z += vel.z;
			cam.target = cam.pos + glm::vec3(0.f, 1.f, -3.f);

			//Do friction
			float _fric = cam.fric * sim->deltaTime;
			vel.x -= _fric * Utility::signum(vel.x);
			if (abs(vel.x) < _fric)
				vel.x = 0;
			vel.y -= _fric * Utility::signum(vel.y);
			if (abs(vel.y) < _fric)
				vel.y = 0;
			vel.z -= _fric * Utility::signum(vel.z);
			if (abs(vel.z) < _fric)
				vel.z = 0;


			//Modify lookat
			cam.lookat = glm::lookAt(cam.pos, cam.target, cam.up);
		}

		//-- General
		else {
			//replace this: phys.pos += phys.vel;
			pos += vel;
		}
		});
}

void init(SmartSim _sim) {
	sim = _sim;
	sim->evw->dispatcher.sink<stepEvent>().connect<step>();
}

SYSTEM_DEF_END