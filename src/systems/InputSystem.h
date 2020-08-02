#pragma once
#include "../sundile/EventWrapper.h"
#include "../components/Input.h"

SYSTEM_DEF_BEGIN(InputSystem)

//TODO - create adjustable keymap structure
//- Probably should make an IO component/system, and pass the component type so it parses the right function
//- enum components ?

void catchInputEvent(const SimInputEvent& ev) {
	ev.registry->view<input>().each([&](auto& entity, input& in) {
		if (ev.action == GLFW_PRESS) {
			switch (ev.key) {
			case(GLFW_KEY_A):
				in.pressed[btn::left] = true;
				break;
			case(GLFW_KEY_D):
				in.pressed[btn::right] = true;
				break;
			case(GLFW_KEY_W):
				in.pressed[btn::up] = true;
				break;
			case(GLFW_KEY_S):
				in.pressed[btn::down] = true;
				break;
			case(GLFW_KEY_Q):
				in.pressed[btn::trigger_left] = true;
				break;
			case(GLFW_KEY_E):
				in.pressed[btn::trigger_right] = true;
				break;

			case(GLFW_MOUSE_BUTTON_LEFT):
				in.pressed[btn::mb_left] = true;
				break;
			case(GLFW_MOUSE_BUTTON_RIGHT):
				in.pressed[btn::mb_right] = true;
				break;
			case(GLFW_MOUSE_BUTTON_MIDDLE):
				in.pressed[btn::mb_middle] = true;
				break;
			};
		}
		if (ev.action == GLFW_RELEASE) {
			switch (ev.key) {
			case(GLFW_KEY_A):
				in.released[btn::left] = true;
				break;
			case(GLFW_KEY_D):
				in.released[btn::right] = true;
				break;
			case(GLFW_KEY_W):
				in.released[btn::up] = true;
				break;
			case(GLFW_KEY_S):
				in.released[btn::down] = true;
				break;
			case(GLFW_KEY_Q):
				in.released[btn::trigger_left] = true;
				break;
			case(GLFW_KEY_E):
				in.released[btn::trigger_right] = true;
				break;

			case(GLFW_MOUSE_BUTTON_LEFT):
				in.released[btn::mb_left] = true;
				break;
			case(GLFW_MOUSE_BUTTON_RIGHT):
				in.released[btn::mb_right] = true;
				break;
			case(GLFW_MOUSE_BUTTON_MIDDLE):
				in.released[btn::mb_middle] = true;
				break;
			};
		}
	});
}
void catchStepEvent(const SimStepEvent& ev) {
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

void catchTypedWindowEvent(const TypedWindowEvent<double>& ev) { //convert to sim event?
	GLFWwindow* win = ev.window;

	int* width = new int;
	int* height = new int;
	glfwGetFramebufferSize(win, width, height);
	//cursorpos = glm::vec2((ev.vals[0]) / (*width), (ev.vals[1]) / (*height)); //normalized :)
	delete width;
	delete height;
}

void init(const SimInitEvent& ev) {
	ev.evw->dispatcher.sink<SimInputEvent>().connect<&catchInputEvent>();
	ev.evw->dispatcher.sink<SimStepEvent>().connect<&catchStepEvent>();
	ev.evw->dispatcher.sink< TypedWindowEvent<double>>().connect<&catchTypedWindowEvent>();
}

SYSTEM_DEF_END