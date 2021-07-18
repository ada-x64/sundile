#ifndef INPUT_TYPES
#define INPUT_TYPES

namespace sundile {
	enum class btn {
		//Gamepad keys
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
		trigger_left, //L2, ZL
		trigger_right, //R2, ZR
		start, //Start button
		select, //Select button / Touch pad

		//Mouse buttons
		mb_left, //left mouse button
		mb_middle, //right mouse button
		mb_right, //middle mouse button / scroll wheel pressed

		//virtual keys
		vk_a,
		vk_b,
		vk_c,
		vk_d,
		vk_e,
		vk_f,
		vk_g,
		vk_h,
		vk_i,
		vk_j,
		vk_k,
		vk_l,
		vk_m,
		vk_n,
		vk_o,
		vk_p,
		vk_q,
		vk_r,
		vk_s,
		vk_t,
		vk_u,
		vk_v,
		vk_w,
		vk_x,
		vk_y,
		vk_z,
		vk_0,
		vk_1,
		vk_2,
		vk_3,
		vk_4,
		vk_5,
		vk_6,
		vk_7,
		vk_8,
		vk_9,

		vk_minus,
		vk_plus,
		vk_tilde,
		vk_leftBracket,
		vk_rightBracket,
		vk_backslash,
		vk_semicolon,
		vk_apostrophe,
		vk_comma,
		vk_period,
		vk_slash,
		vk_leftShift,
		vk_rightShift,
		vk_leftCtrl,
		vk_rightCtrl,
		vk_spacebar,
		vk_leftAlt,
		vk_rightAlt,
		vk_escape,
		vk_up,
		vk_down,
		vk_left,
		vk_right,
		vk_insert,
		vk_home,
		vk_pageUp,
		vk_pageDown,
		vk_delete,
		vk_end,
		vk_leftSuper,
		vk_rightSuper,
		vk_tab,
		vk_enter,

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
		{btn::mb_right, GLFW_MOUSE_BUTTON_RIGHT},
		
		{btn::vk_a, GLFW_KEY_A},
		{btn::vk_b, GLFW_KEY_B},
		{btn::vk_c, GLFW_KEY_C},
		{btn::vk_d, GLFW_KEY_D},
		{btn::vk_e, GLFW_KEY_E},
		{btn::vk_f, GLFW_KEY_F},
		{btn::vk_g, GLFW_KEY_G},
		{btn::vk_h, GLFW_KEY_H},
		{btn::vk_i, GLFW_KEY_I},
		{btn::vk_j, GLFW_KEY_J},
		{btn::vk_k, GLFW_KEY_K},
		{btn::vk_l, GLFW_KEY_L},
		{btn::vk_m, GLFW_KEY_M},
		{btn::vk_n, GLFW_KEY_N},
		{btn::vk_o, GLFW_KEY_O},
		{btn::vk_p, GLFW_KEY_P},
		{btn::vk_q, GLFW_KEY_Q},
		{btn::vk_r, GLFW_KEY_R},
		{btn::vk_s, GLFW_KEY_S},
		{btn::vk_t, GLFW_KEY_T},
		{btn::vk_u, GLFW_KEY_U},
		{btn::vk_v, GLFW_KEY_V},
		{btn::vk_w, GLFW_KEY_W},
		{btn::vk_x, GLFW_KEY_X},
		{btn::vk_y, GLFW_KEY_Y},
		{btn::vk_z, GLFW_KEY_Z},
		{btn::vk_0, GLFW_KEY_0},
		{btn::vk_1, GLFW_KEY_1},
		{btn::vk_2, GLFW_KEY_2},
		{btn::vk_3, GLFW_KEY_3},
		{btn::vk_4, GLFW_KEY_4},
		{btn::vk_5, GLFW_KEY_5},
		{btn::vk_6, GLFW_KEY_6},
		{btn::vk_7, GLFW_KEY_7},
		{btn::vk_8, GLFW_KEY_8},
		{btn::vk_9, GLFW_KEY_9},

		{btn::vk_minus, GLFW_KEY_MINUS},
		{btn::vk_plus, GLFW_KEY_EQUAL},
		{btn::vk_tilde, GLFW_KEY_GRAVE_ACCENT},
		{btn::vk_leftBracket, GLFW_KEY_LEFT_BRACKET},
		{btn::vk_rightBracket, GLFW_KEY_RIGHT_BRACKET},
		{btn::vk_backslash, GLFW_KEY_BACKSLASH},
		{btn::vk_semicolon, GLFW_KEY_SEMICOLON},
		{btn::vk_apostrophe, GLFW_KEY_APOSTROPHE},
		{btn::vk_comma, GLFW_KEY_COMMA},
		{btn::vk_period, GLFW_KEY_PERIOD},
		{btn::vk_slash, GLFW_KEY_SLASH},
		{btn::vk_leftShift, GLFW_KEY_LEFT_SHIFT},
		{btn::vk_rightShift, GLFW_KEY_RIGHT_SHIFT},
		{btn::vk_leftCtrl, GLFW_KEY_LEFT_CONTROL},
		{btn::vk_rightCtrl, GLFW_KEY_RIGHT_CONTROL},
		{btn::vk_spacebar, GLFW_KEY_SPACE},
		{btn::vk_leftAlt, GLFW_KEY_LEFT_ALT},
		{btn::vk_rightAlt, GLFW_KEY_RIGHT_ALT},
		{btn::vk_escape, GLFW_KEY_ESCAPE},
		{btn::vk_up, GLFW_KEY_UP},
		{btn::vk_down, GLFW_KEY_DOWN},
		{btn::vk_left, GLFW_KEY_LEFT},
		{btn::vk_right, GLFW_KEY_RIGHT},
		{btn::vk_insert, GLFW_KEY_INSERT},
		{btn::vk_home, GLFW_KEY_HOME},
		{btn::vk_pageUp, GLFW_KEY_PAGE_UP},
		{btn::vk_pageDown, GLFW_KEY_PAGE_DOWN},
		{btn::vk_delete, GLFW_KEY_DELETE},
		{btn::vk_end, GLFW_KEY_END},
		{btn::vk_leftSuper, GLFW_KEY_LEFT_SUPER},
		{btn::vk_rightSuper, GLFW_KEY_RIGHT_SUPER},
		{btn::vk_tab, GLFW_KEY_TAB},
		{btn::vk_enter, GLFW_KEY_ENTER}
	};

	Input& operator =(Input& other) {
		for (int i = 0; i < (int)(btn::COUNT); ++i) {
			this->pressed[i] = other.pressed[i];
			this->held[i] = other.held[i];
			this->released[i] = other.released[i];
		}

		this->cursorpos = other.cursorpos;
		this->cursorpos_prev = other.cursorpos_prev;
		this->map = other.map;
		this->locked = other.locked;
		this->disabled = other.disabled;

		return *this;
	}

END_COMPONENT //Input

#endif