#pragma once

#ifndef SUNDILE_INPUT
#define	SUNDILE_INPUT
#include "../EventSystem/EventSystem.h"

SYSTEM(InputSystem)
	using namespace Components;

	namespace { //private
		inline Input current;
	}

	bool isHeld(btn key) {
		return current.held[static_cast<int>(key)];
	}
	bool isPressed(btn key) {
		return current.pressed[static_cast<int>(key)];
	}
	bool isReleased(btn key) {
		return current.released[static_cast<int>(key)];
	}

	class cursorStat {
		Input current;
	public:
		cursorStat(Input current) : current(current) {};
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

	void guiEvent(const GuiEvent& ev) {
		ev;
		ev.registry->view<Input>().each([&ev](auto& entity, Input& c) {
			c.disabled = ev.payload.at("disable input");
		});
	}
	void inputEvent(const SceneInputEvent& ev) {
		if (ev.action != GLFW_PRESS && ev.action != GLFW_RELEASE) return;
		ev.registry->view<Input>().each([&](auto& entity, Input& in) {
			if (in.disabled || in.locked) return;
			auto map = in.map;
			bool* valueToChange = ev.action == GLFW_PRESS ? in.pressed : in.released;
			for (auto pair : map) {
				if (ev.key == pair.second) {
					valueToChange[static_cast<int>(pair.first)] = true;
					return;
				}
			}
		});
	}
	void StepEvent(const SceneStepEvent& ev) {
		ev.registry->view<Input>().each([&](auto& entity, Input& in) {
			updateGUI<Input>(entity, in);
			if (in.disabled) {
				for (int i = 0; i < static_cast<int>(btn::COUNT); ++i) {
					in.pressed[i] = false;
					in.held[i] = false;
					in.released[i] = false;
				}
				in.cursorpos_prev = -1;
				in.cursorpos = -1;
			}
			else {

				//cursor input
				in.window = WindowSystem::currentWindow->window;
				if (in.window == nullptr) return;
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

				for (int i = 0; i < static_cast<int>(btn::COUNT); ++i) {
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
		Input* in = meta_cast<Input>(meta);

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
			int key = static_cast<int>(pair.first);
			auto val = pair.second;
			ImGui::Text("%s: %s", nameMap[pair.first], getKeyNameByValue(val));
			auto drawlist = GetWindowDrawList();
			auto p = ImGui::GetCursorScreenPos();
			drawlist->AddRectFilled({ p.x - 8,p.y - 8 }, { p.x + 8,p.y + 8 },
				ImGui::ColorConvertFloat4ToU32({
					static_cast<float>(in->pressed[key]),
					static_cast<float>(in->held[key]),
					static_cast<float>(in->released[key]),
					1
				}));

		}
	}

	void init(const InitEvent& ev) {
		ev.evw->dispatcher.sink<SceneInputEvent>().connect<&inputEvent>();
		ev.evw->dispatcher.sink<SceneStepEvent>().connect<&StepEvent>();
		ev.evw->dispatcher.sink<GuiEvent>().connect<&guiEvent>();

		defineGui<Input>(drawGui);
	}
END_SYSTEM
#endif