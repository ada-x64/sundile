//--
//-- Window.h
//--
#pragma once
#ifndef WINDOW_H
#define WINDOW_H
#include "Systems.h"

namespace sundile {
	//Namespace for GLFW functionality
	namespace Window
	{
		//--
		//-- Window definition
		//--
		inline GLFWwindow* window;
		inline int WIDTH = 800;
		inline int HEIGHT = 600;

		//--
		//-- Events
		//--
		void handleKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);

		//--
		//--Initialize GLFW
		//--
		void init();

		//--
		//-- Update in the main loop
		//--
		void update();

	}
}
#endif