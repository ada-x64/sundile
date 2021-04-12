#ifndef INPUT_TYPES
#define INPUT_TYPES

namespace sundile {
	enum class btn {
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

COMPONENT(Input)
bool pressed[static_cast<int>(btn::COUNT)];
bool held[static_cast<int>(btn::COUNT)];
bool released[static_cast<int>(btn::COUNT)];
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
END_COMPONENT //Input

#endif