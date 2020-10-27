#pragma once
#include "../sundile/sundile.h"
#include "../components/Camera.h"


BEGIN_SYSTEM(CameraSystem)

	glm::vec3 rotatexy(glm::vec3 vec, float radians) {
		//note: this is the passive transform, used for rotating axes
		vec.x = vec.x * cosf(radians) + vec.y * sinf(radians);
		vec.y = -vec.x * sinf(radians) + vec.y * cosf(radians);
		return vec;
	}

	void stepEvent(const SimStepEvent& ev) {
		using namespace glm;
		ev.registry->view<camera>().each([&](auto entity, camera& cam) {
			updateGUI<camera>(entity, cam);

			//
			//Identity and inverse.
			mat4 T = mat4(1.f);
			mat4 Mi = inverse(cam.T);
			Vec3& dir = cam.dir;
			Vec3& pos = cam.pos;
			Vec3& spd = cam.spd;
			float pi = glm::pi<float>();

			//
			//Rotation
			vec3 newdir = vec3(0.f);
			if (InputSystem::cursorStatus().isChanged()) {
				float scale = pi;
				Vec2 delta = InputSystem::cursorStatus().getDelta();

				if (InputSystem::isHeld(btn::mb_left)) {
					float radians = delta.x * scale;
					newdir.y += radians;
				}
				if (InputSystem::isHeld(btn::mb_right)) {
					float radians = delta.y * scale;
					newdir.x += radians;
				}
			}
			dir += newdir;

			float xlim = pi / 2.f - pi / 16.f;
			if (abs(dir.x) > xlim) {
				dir.x = xlim * sign(dir.x);
			}

			T = rotate(T, dir.x, vec3(1, 0, 0));
			T = rotate(T, dir.y, vec3(0, 1, 0));
			T = rotate(T, dir.z, vec3(0, 0, 1));

			//
			//Translation
			float movspd = -.1f;
			vec3 newspd = vec3(0.f);
			if (InputSystem::isHeld(btn::left)) {
				newspd.x -= movspd;
			}
			if (InputSystem::isHeld(btn::right)) {
				newspd.x += movspd;
			}
			if (InputSystem::isHeld(btn::up)) {
				newspd.z -= movspd;
			}
			if (InputSystem::isHeld(btn::down)) {
				newspd.z += movspd;
			}
			if (InputSystem::isHeld(btn::trigger_left)) {
				newspd.y -= movspd;
			}
			if (InputSystem::isHeld(btn::trigger_right)) {
				newspd.y += movspd;
			}
			if (Input.held[btn::up]) {
				newspd.z -= movspd;
			}
			if (Input.held[btn::down]) {
				newspd.z += movspd;
			}
			if (Input.held[btn::trigger_left]) {
				newspd.y -= movspd;
			}
			if (Input.held[btn::trigger_right]) {
				newspd.y += movspd;
			}

			//Clamp speed, do friction (probably a better way to do this)
			float absx = abs(newspd.x);
			newspd.x -= sign(newspd.x) * cam.fric;
			if (absx < abs(cam.fric)) newspd.x = 0.f;
			if (absx > cam.maxspd) newspd.x = absx * cam.maxspd;

			float absy = abs(newspd.y);
			newspd.y -= sign(newspd.y) * cam.fric;
			if (abs(newspd.y) < abs(cam.fric)) newspd.y = 0.f;
			if (absy > cam.maxspd) newspd.y = absy * cam.maxspd;

			//Clamp speed, do friction (probably a better way to do this)
			float absx = abs(newspd.x);
			newspd.x -= sign(newspd.x) * cam.fric;
			if (absx < abs(cam.fric)) newspd.x = 0.f;
			if (absx > cam.maxspd) newspd.x = absx * cam.maxspd;

			float absy = abs(newspd.y);
			newspd.y -= sign(newspd.y) * cam.fric;
			if (abs(newspd.y) < abs(cam.fric)) newspd.y = 0.f;
			if (absy > cam.maxspd) newspd.y = absy * cam.maxspd;

			float absz = abs(newspd.z);
			newspd.z -= sign(newspd.z) * cam.fric;
			if (abs(newspd.z) < abs(cam.fric)) newspd.z = 0.f;
			if (absz > cam.maxspd) newspd.z = absz * cam.maxspd;

			//Do the translation
			newspd = rotateY(newspd, -dir.y);
			cam.spd += newspd;
			T = translate(T, vec3(cam.spd));

			//Set MVP
			cam.T *= Mi * T;
		});
	}

	void init(const SimInitEvent& ev) {
		ev.evw->dispatcher.sink<SimStepEvent>().connect<&stepEvent>();

		//dependencies
		ev.registry->on_construct<camera>().connect<&entt::registry::emplace_or_replace<velocity>>();

		defineGui<camera>([](guiMeta& meta) {
			using namespace ImGui;
			camera* c = meta_cast<camera>(meta);
			DragFloat("maxspd", &(c->maxspd));
		});
	}

END_SYSTEM