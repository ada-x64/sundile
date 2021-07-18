#pragma once

#ifndef SUNDILE_INPUT
#define	SUNDILE_INPUT
#include "../EventSystem/EventSystem.h"

SYSTEM(InputSystem)
	using namespace Components;

	namespace { //private
		inline Input current;
	}

	bool isHeld(Input& in, btn key) {
		return in.held[static_cast<int>(key)];
	}
	bool isPressed(Input& in, btn key) {
		return in.pressed[static_cast<int>(key)];
	}
	bool isReleased(Input& in, btn key) {
		return in.released[static_cast<int>(key)];
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
	cursorStat cursorStatus(Input& in) {
		return cursorStat(current);
	}
	void applyMap(Input& in, inputMap map) {
		in.map = map;
	}

	//Updates input status e.g. from pressed -> held -> released
	void update(Input& in) {
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
			//in.cursorpos_prev = in.cursorpos;
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
		}
	}

	void postStepEvent(PostStepEvent<SmartEVW> ev) {
		update(WindowSystem::currentWindow->input);
	}
	void sceneStep(PreStepEvent<SmartScene> ev) {
		update(ev.member->input);
	}
	void scenePostStep(PostStepEvent<SmartScene> ev) {
		ev.member->input.cursorpos_prev = ev.member->input.cursorpos;
	}

	const char* getKeyNameByValue(int val) {
		return val == GLFW_MOUSE_BUTTON_LEFT ? "left mouse button" : val == GLFW_MOUSE_BUTTON_RIGHT ? "right mouse button" : val == GLFW_MOUSE_BUTTON_MIDDLE ? "middle mouse button" :
			val == GLFW_KEY_UP ? "up arrow" : val == GLFW_KEY_DOWN ? "down arrow" : val == GLFW_KEY_LEFT ? "left arrow" : val == GLFW_KEY_RIGHT ? "right arrow" :
			val == GLFW_KEY_RIGHT_CONTROL ? "ctrl (right)" : val == GLFW_KEY_RIGHT_ALT ? "alt (right)" : val == GLFW_KEY_RIGHT_SHIFT ? "shift (right)" :
			val == GLFW_KEY_LEFT_CONTROL ? "ctrl (left)" : val == GLFW_KEY_LEFT_ALT ? "alt (left)" : val == GLFW_KEY_LEFT_SHIFT ? "shift (left)" :
			val == GLFW_KEY_ENTER ? "enter" : val == GLFW_KEY_ESCAPE ? "esc" :
			glfwGetKeyName(val, -1);
	}

	void interpretInput(Input& in, int key, int scancode, int action, int mods) {
		if (action != GLFW_PRESS && action != GLFW_RELEASE) return;
		if (in.disabled || in.locked) return;
		auto map = in.map;
		bool* valueToChange = action == GLFW_PRESS ? in.pressed : in.released;
		for (auto pair : map) {
			if (key == pair.second) {
				valueToChange[static_cast<int>(pair.first)] = true;
				return;
			}
		}
	}
	void interpretCursorPos(Input& in, Vec2 pos) {
		in.cursorpos_prev = in.cursorpos;
		in.cursorpos = pos;
	}

	void init(const InitEvent<SmartEVW>& ev) {
		ev.member->dispatcher.sink<PostStepEvent<SmartEVW>>().connect<&postStepEvent>();
		ev.member->dispatcher.sink<PreStepEvent<SmartScene>>().connect<&sceneStep>();
		ev.member->dispatcher.sink<PostStepEvent<SmartScene>>().connect<&scenePostStep>();
	}
END_SYSTEM
#endif