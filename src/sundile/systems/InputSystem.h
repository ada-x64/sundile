#pragma once

#ifndef SUNDILE_INPUT
#define	SUNDILE_INPUT

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

	typedef std::map<const btn, int> inputMap;
}

BEGIN_COMPONENT(input)
	bool pressed[btn::COUNT];
	bool held[btn::COUNT];
	bool released[btn::COUNT];
	glm::vec2 cursorpos;
	glm::vec2 cursorpos_prev;
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
			input current;
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
		void applyMap(inputMap map) {
			current.map = map;
		}

		void inputEvent(const SimInputEvent& ev) {
			if (ev.key != GLFW_PRESS && ev.key != GLFW_RELEASE) return;
			ev.registry->view<input>().each([&](auto& entity, input& in) {
				auto map = in.map;
				auto valueToChange = ev.action == GLFW_PRESS ? in.pressed : in.released;
				for (auto pair : map) {
					if (ev.key == pair.second) {
						valueToChange[pair.first] = true;
					}
				}
			});
		}
		void stepEvent(const SimStepEvent& ev) {
			ev.registry->view<input>().each([&](auto& entity, input& in) {
				for (int i = 0; i < btn::COUNT; ++i) {
					if (in.released[i] && !in.held[i]) {
						in.released[i] = false;
					}
					if (in.released[i] && in.held[i]) {
						in.held[i] = false;
					}
					if (in.pressed[i] && in.held[i]) {
						in.pressed[i] = false;
					}
					if (in.pressed[i]) {
						in.held[i] = true;
					}
				}
				});
		}
		void typedWindowEvent(const TypedWindowEvent<double>& ev) { //convert to sim event?
			GLFWwindow* win = ev.window;

			int* width = new int;
			int* height = new int;
			glfwGetFramebufferSize(win, width, height);
			//cursorpos = glm::vec2((ev.vals[0]) / (*width), (ev.vals[1]) / (*height)); //normalized :)
			delete width;
			delete height;
		}
		void init(const SimInitEvent& ev) {
			ev.evw->dispatcher.sink<SimInputEvent>().connect<&inputEvent>();
			ev.evw->dispatcher.sink<SimStepEvent>().connect<&stepEvent>();
			ev.evw->dispatcher.sink< TypedWindowEvent<double>>().connect<&typedWindowEvent>();


			auto eInput = ev.registry->create();
			emplace<input>(ev.registry, eInput, current);

			defineGui<input>([&](GuiSystem::guiMeta& meta) {
				//TODO: allow user to modify input map and view input status
				//TODO: debug - why did input stop working? Something to do with the emplacement above?
			});
		}
	}
}
#endif