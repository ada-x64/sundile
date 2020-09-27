#pragma once
#include "../sundile/sundile.h"
#include "../components/Camera.h"
#include "../components/Input.h"


BEGIN_SYSTEM(CameraSystem)
	inline glm::vec2 cursorpos;
	inline glm::vec2 cursorpos_prev;


	glm::vec3 rotatexy(glm::vec3 vec, float radians) {
		//note: this is the passive transform, used for rotating axes
		vec.x = vec.x * cosf(radians) + vec.y * sinf(radians);
		vec.y = -vec.x * sinf(radians) + vec.y * cosf(radians);
		return vec;
	}

	void catchCursorEvent(const TypedWindowEvent<double>& ev) { //TODO - move this to InputSystem
		GLFWwindow* win = ev.window;
	
		int* width = new int;
		int* height = new int;
		glfwGetFramebufferSize(win, width, height);

		cursorpos = glm::vec2((ev.vals[0])/(*width), (ev.vals[1])/(*height)); //normalized :)
		delete width;
		delete height;
	}

	void catchGuiEvent(const GuiEvent ev) {
		//TODO: lock controls when window is out of focus
		//printf("Gui event caught! Event: %f", ev.content);
	}

	void catchStepEvent(const SimStepEvent& ev) {
		using namespace glm;
		ev.registry->view<camera, input>().each([&](auto entity, camera& cam, input& Input) {

			//
			//Identity and inverse.
			mat4 T = mat4(1.f);
			mat4 Mi = inverse(cam.model);
			Vec3& dir = cam.dir;
			Vec3& pos = cam.pos;
			Vec3& spd = cam.spd;
			float pi = glm::pi<float>();

			//
			//Rotation
			vec3 newdir = vec3(0.f);
			if (cursorpos != cursorpos_prev) {
				float scale = pi;

				if (Input.held[btn::mb_left]) {
					float xdif = (cursorpos.x - cursorpos_prev.x);
					float radians = xdif * scale;
					newdir.y += radians;
				}
				if (Input.held[btn::mb_right]) {
					float ydif = (cursorpos.y - cursorpos_prev.y);
					float radians = ydif * scale;
					newdir.x += radians;
				}

				/**
				cam.model = glm::translate(
					glm::rotate(
						glm::translate(cam.model, -cam.pos),
						radians, axis),
					cam.pos);
				/**/
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
			if (Input.held[btn::left]) {
				newspd.x -= movspd;
			}
			if (Input.held[btn::right]) {
				newspd.x += movspd;
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

			float absz = abs(newspd.z);
			newspd.z -= sign(newspd.z) * cam.fric;
			if (abs(newspd.z) < abs(cam.fric)) newspd.z = 0.f;
			if (absz > cam.maxspd) newspd.z = absz * cam.maxspd;

			//Do the translation
			newspd = rotateY(newspd, -dir.y);
			cam.spd += newspd;
			T = translate(T, vec3(cam.spd));

			//set cursor position
			cursorpos_prev = cursorpos;

			//Set MVP
			cam.model *= Mi * T;
			cam.mvp = cam.model;

			//Log
			/**
			std::string Spos = "pos = (%f, %f, %f)\n";
			std::string Sspd = "spd = (%f,%f,%f)\n";
			std::string Sfront = "front = (%f,%f,%f)\n";
			std::string Sdir = "dir = (%f, %f, %f)\n";
			std::string Smvp = "mvp = (%f, %f, %f, %f)\n";
			std::string ST = "T = (%f, %f, %f, %f)\n";
			std::string SMi = "Mi = (%f, %f, %f, %f)\n";
			std::string Sfinal = Spos + Sspd + Sfront + Sdir + Smvp + Smvp + Smvp + Smvp + ST + ST + ST + ST + SMi + SMi + SMi + SMi + '\n';
			printf(Sfinal.c_str(),
				cam.pos.x, cam.pos.y, cam.pos.z,
				newspd.x, newspd.y, newspd.z,
				cam.front.x, cam.front.y, cam.front.z,
				cam.dir.x, cam.dir.y, cam.dir.z,
				cam.mvp[0][0], cam.mvp[0][1], cam.mvp[0][2], cam.mvp[0][3],
				cam.mvp[1][0], cam.mvp[1][1], cam.mvp[1][2], cam.mvp[1][3],
				cam.mvp[2][0], cam.mvp[2][1], cam.mvp[2][2], cam.mvp[2][3],
				cam.mvp[3][0], cam.mvp[3][1], cam.mvp[3][2], cam.mvp[3][3],
				T[0][0], T[0][1], T[0][2], T[0][3],
				T[1][0], T[1][1], T[1][2], T[1][3],
				T[2][0], T[2][1], T[2][2], T[2][3],
				T[3][0], T[3][1], T[3][2], T[3][3],
				Mi[0][0], Mi[0][1], Mi[0][2], Mi[0][3],
				Mi[1][0], Mi[1][1], Mi[1][2], Mi[1][3],
				Mi[2][0], Mi[2][1], Mi[2][2], Mi[2][3],
				Mi[3][0], Mi[3][1], Mi[3][2], Mi[3][3]
			);
			/**/
		});
	}

	void init(const SimInitEvent& ev) {
		ev.evw->dispatcher.sink<SimStepEvent>().connect<&catchStepEvent>();
		ev.evw->dispatcher.sink< TypedWindowEvent<double>>().connect<&catchCursorEvent>();
		ev.evw->dispatcher.sink<GuiEvent>().connect<&catchGuiEvent>();

		//dependencies
		ev.registry->on_construct<camera>().connect<&entt::registry::emplace_or_replace<velocity>>();

		defineGui<camera>([](entt::meta_any& _c) { //TODO: Allow value changes.
			using namespace ImGui;
			camera* c = static_cast<camera*>(_c.data());
			DragFloat3("pos", c->pos); //NOTE: Likely has to do with conversion from sundile::Vec3 to float*
			DragFloat3("front", c->front);
			DragFloat3("dir", c->dir);
		});
	}

END_SYSTEM