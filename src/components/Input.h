#pragma once
#include "../sundile/sundile.h"
//TODO - probably should make this a part of sundile instead of an independent component
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
		mb_left, //left mouse button
		mb_middle, //right mouse button
		mb_right, //middle mouse button / scroll wheel pressed
		COUNT
	};
}

BEGIN_COMPONENT(input)

	bool pressed[btn::COUNT];
	bool held[btn::COUNT];
	bool released[btn::COUNT];
	glm::vec2 cursorpos;
	glm::vec2 cursorpos_prev;

	
END_COMPONENT
