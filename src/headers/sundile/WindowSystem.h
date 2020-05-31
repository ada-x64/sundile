//--
//-- Window.h
//--
#pragma once
#ifndef WINDOW_H
#define WINDOW_H
#include "EventWrapper.h"

namespace sundile {

	struct SmartGLFWwindowDestroyer {
		void operator()(GLFWwindow* ptr) {
			glfwDestroyWindow(ptr);
		}
	};
	typedef std::unique_ptr<GLFWwindow, SmartGLFWwindowDestroyer> SmartGLFWwindow;

	struct WindowContainer
	{
		SmartGLFWwindow	window;
		SmartEVW		evw;
		int				WIDTH				= 800;
		int				HEIGHT				= 600;
		bool			windowShouldClose	= false;
		int				vecIndex; //tradeoff: iterate each time you need access, or reassign each at init/termination?

		bool operator == (WindowContainer & other) {
			return (this->window == other.window);
		}
	};
	typedef std::shared_ptr<WindowContainer> SmartWindow;


	namespace WindowSystem {
		inline SmartWindow currentwindow;
		inline SmartEVW	currentevw;
		inline bool	GLFWinitialized = false;
		inline bool	GLEWinitialized = false;
		inline std::vector<SmartWindow> windows	= std::vector<SmartWindow>();

		void initGLFW();

		SmartWindow init(SmartEVW evw);
		SmartWindow init(SmartEVW evw, SmartWindow winc);

		SmartWindow update(SmartWindow winc);
		void updateAll();

		void terminate(SmartWindow winc);
	}
}
#endif