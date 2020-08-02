#pragma once
#include "../sundile/Components.h"

//Default captured buttons.
#ifndef SUNDILE_BTNS
#define SUNDILE_BTNS
namespace sundile {
	enum btn {
		up,
		down,
		left,
		right,
		pad_up,
		pad_down,
		pad_left,
		pad_right,
		bumper_left,
		bumper_right,
		trigger_left,
		trigger_right,
		mb_left,
		mb_middle,
		mb_right,
		COUNT
	};
}
#endif

//TODO - probably should make this a part of sundile instead of an independent component
COMPONENT_DEF_BEGIN(input)

bool pressed[btn::COUNT];
bool held[btn::COUNT];
bool released[btn::COUNT];
glm::vec2 cursorpos;
glm::vec2 cursorpos_prev;


COMPONENT_DEF_END