#pragma once

#ifndef SUNDILE_INPUT
#define	SUNDILE_INPUT
#include "EventSystem/EventSystem.h"

namespace sundile {
	enum btn {
		up, //keyboard up
		down, //keyboard down
		left, //keyboard left
		right, //keyboard right
		btn1, //Y, Triangle
		btn2, //B, Circle
		btn3, //A, Cross
		btn4, //X, Square
		btn5, //L3
		btn6, //R3
		pad_up, //dpad up
		pad_down, //dpad down
		pad_left, //dpad left
		pad_right, //dpad right
		bumper_left, //L1
		bumper_right, //R1
		trigger_left, //L2
		trigger_right, //R2
		start, //Start button
		select, //Select button / Touch pad
		mb_left, //left mouse button
		mb_middle, //right mouse button
		mb_right, //middle mouse button / scroll wheel pressed
		COUNT
	};

	typedef std::multimap<btn, int> inputMap;
}

BEGIN_COMPONENT(input)
	bool pressed[btn::COUNT];
	bool held[btn::COUNT];
	bool released[btn::COUNT];
	Vec2 cursorpos;
	Vec2 cursorpos_prev;
	GLFWwindow* window;
	bool locked = false;   //continues current state
	bool disabled = false; //resets all keys
	inputMap map = {
		{btn::up, GLFW_KEY_W},
		{btn::down, GLFW_KEY_S},
		{btn::left, GLFW_KEY_A},
		{btn::right, GLFW_KEY_D},
		{btn::btn1, GLFW_KEY_O},
		{btn::btn2, GLFW_KEY_SEMICOLON},
		{btn::btn3, GLFW_KEY_L},
		{btn::btn4, GLFW_KEY_K},
		{btn::pad_up, GLFW_KEY_UP},
		{btn::pad_down, GLFW_KEY_DOWN},
		{btn::pad_left, GLFW_KEY_LEFT},
		{btn::pad_right, GLFW_KEY_RIGHT},
		{btn::bumper_left, GLFW_KEY_Q},
		{btn::bumper_right, GLFW_KEY_E},
		{btn::trigger_left, GLFW_KEY_1},
		{btn::trigger_right, GLFW_KEY_3},
		{btn::start, GLFW_KEY_ENTER},
		{btn::select, GLFW_KEY_RIGHT_CONTROL},
		{btn::mb_left, GLFW_MOUSE_BUTTON_LEFT},
		{btn::mb_middle, GLFW_MOUSE_BUTTON_MIDDLE},
		{btn::mb_right, GLFW_MOUSE_BUTTON_RIGHT}
	};
END_COMPONENT //input

namespace sundile {
	namespace InputSystem {
		using namespace Components;

		namespace { //private
			inline input current;
		}

		bool isHeld(btn key) {
			return current.held[key];
		}
		bool isPressed(btn key) {
			return current.pressed[key];
		}
		bool isReleased(btn key) {
			return current.released[key];
		}

		class cursorStat {
			input current;
		public:
			cursorStat(input current) : current(current) {};
			bool isChanged() {
				return current.cursorpos != current.cursorpos_prev;
			}
			Vec2 getDelta() {
				return current.cursorpos - current.cursorpos_prev;
			}
		};
		cursorStat cursorStatus() {
			return cursorStat(current);
		}
		void applyMap(inputMap map) {
			current.map = map;
		}

		//TODO: this is breaking - something with the initializer list being wrong?
		//WWID: Removing sim and winc references from GUI initialization - should only need evw
		//- May have something to do with the way I redid GuiEvent, or passing const entt::registry*'s.
		void guiEvent(const GuiEvent& ev) {
			if (ev.payload.key == GuiStateKey::focusAny) {
				ev.registry->view<input>().each([&ev](auto& entity, input& c) {
					c.disabled = ev.payload.value;
				});
			}
		}
		void inputEvent(const SimInputEvent& ev) {
			if (ev.action != GLFW_PRESS && ev.action != GLFW_RELEASE) return;
			ev.registry->view<input>().each([&](auto& entity, input& in) {
				if (in.disabled || in.locked) return;
				auto map = in.map;
				bool* valueToChange = ev.action == GLFW_PRESS ? in.pressed : in.released;
				for (auto pair : map) {
					if (ev.key == pair.second) {
						valueToChange[pair.first] = true;
						return;
					}
				}
			});
		}
		void stepEvent(const SimStepEvent& ev) {
			ev.registry->view<input>().each([&](auto& entity, input& in) {
				updateGUI<input>(entity, in);
				if (in.disabled) {
					for (int i = 0; i < btn::COUNT; ++i) {
						in.pressed[i] = false;
						in.held[i] = false;
						in.released[i] = false;
					}
					in.cursorpos_prev = -1;
					in.cursorpos = -1;
				}
				else {

					//cursor input
					in.window = current.window;
					int* width = new int;
					int* height = new int;
					double* xpos = new double;
					double* ypos = new double;
					glfwGetFramebufferSize(in.window, width, height);
					glfwGetCursorPos(in.window, xpos, ypos);
					in.cursorpos_prev = in.cursorpos;
					in.cursorpos = glm::vec2((*xpos) / (*width), (*ypos) / (*height)); //normalized :)
					delete width;
					delete height;
					delete xpos;
					delete ypos;

					for (int i = 0; i < btn::COUNT; ++i) {
						if (in.released[i] && !in.held[i]) {
							in.released[i] = false;
						}
						if (in.released[i] && in.held[i]) {
							in.held[i] = false;
							in.pressed[i] = false;
						}
						if (in.pressed[i] && in.held[i]) {
							in.pressed[i] = false;
						}
						if (in.pressed[i]) {
							in.held[i] = true;
						}
					}
					current = in; //bad
				}
				});
		}


		const char* getKeyNameByValue(int val) {
			return val == GLFW_MOUSE_BUTTON_LEFT ? "left mouse button" : val == GLFW_MOUSE_BUTTON_RIGHT ? "right mouse button" : val == GLFW_MOUSE_BUTTON_MIDDLE ? "middle mouse button" :
				val == GLFW_KEY_UP ? "up arrow" : val == GLFW_KEY_DOWN ? "down arrow" : val == GLFW_KEY_LEFT ? "left arrow" : val == GLFW_KEY_RIGHT ? "right arrow" :
				val == GLFW_KEY_RIGHT_CONTROL ? "ctrl (right)" : val == GLFW_KEY_RIGHT_ALT ? "alt (right)" : val == GLFW_KEY_RIGHT_SHIFT ? "shift (right)" :
				val == GLFW_KEY_LEFT_CONTROL ? "ctrl (left)" : val == GLFW_KEY_LEFT_ALT ? "alt (left)" : val == GLFW_KEY_LEFT_SHIFT ? "shift (left)" :
				val == GLFW_KEY_ENTER ? "enter" : val == GLFW_KEY_ESCAPE ? "esc" :
				glfwGetKeyName(val, -1);
		}
		void drawGui(const guiMeta& meta) {
			using namespace ImGui;
			input* in = meta_cast<input>(meta);

			std::map<const btn, const char*> nameMap = {
				{btn::up, "up"},
				{btn::down, "down"},
				{btn::left, "left"},
				{btn::right, "right"},
				{btn::btn1, "btn1"},
				{btn::btn2, "btn2"},
				{btn::btn3, "btn3"},
				{btn::btn4, "btn4"},
				{btn::pad_up, "pad_up"},
				{btn::pad_down, "pad_down"},
				{btn::pad_left, "pad_left"},
				{btn::pad_right, "pad_right"},
				{btn::bumper_left, "bumper_left"},
				{btn::bumper_right, "bumper_right"},
				{btn::trigger_left, "trigger_left"},
				{btn::trigger_right, "trigger_right"},
				{btn::start, "start"},
				{btn::select, "select"},
				{btn::mb_left, "mb_left"},
				{btn::mb_middle, "mb_middle"},
				{btn::mb_right, "mb_right"}
			};


			Checkbox("Locked", &(in->locked));
			Checkbox("Disabled", &(in->disabled));
			Text("Cursorpos: %f, %f", in->cursorpos.x, in->cursorpos.y);

			for (auto pair : in->map) {
				auto key = pair.first;
				auto val = pair.second;
				ImGui::Text("%s: %s", nameMap[key], getKeyNameByValue(val));
				auto drawlist = GetWindowDrawList();
				auto p = ImGui::GetCursorScreenPos();
				drawlist->AddRectFilled({ p.x - 8,p.y - 8 }, { p.x + 8,p.y + 8 }, ImGui::ColorConvertFloat4ToU32({ static_cast<float>(in->pressed[key]), static_cast<float>(in->held[key]), static_cast<float>(in->released[key]), 1 }));
			}
		}

		void windowInit(const WindowInitEvent& ev) {
			current.window = ev.window; //HACKY AS FUCK - TODO: REPLACE THIS
		}

		void init(const SimInitEvent& ev) {

			ev.evw->dispatcher.sink<SimInputEvent>().connect<&inputEvent>();
			ev.evw->dispatcher.sink<SimStepEvent>().connect<&stepEvent>();
			ev.evw->dispatcher.sink<GuiEvent>().connect<&guiEvent>();

			auto eInput = ev.registry->create();
			current = emplace<input>(ev.registry, eInput);

			defineGui<input>(drawGui);
		}
	}
}
#endif