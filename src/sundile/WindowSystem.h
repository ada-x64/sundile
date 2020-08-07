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
		const char*		name				= "sundile";

		bool operator == (WindowContainer & other) {
			return (this->window.get() == other.window.get());
		}
	};
	typedef std::shared_ptr<WindowContainer> SmartWindow;


	namespace WindowSystem {
		inline SmartWindow emptywindow = std::make_shared<WindowContainer>();
		inline SmartWindow currentwindow;
		inline SmartEVW	currentevw;
		inline bool	GLFWinitialized = false;
		inline bool	GLEWinitialized = false;
		inline bool termination_called = false;
		inline std::vector<SmartWindow> windows	= std::vector<SmartWindow>();

		//-- Helpers
		SmartWindow getSmartWindow(GLFWwindow* w);
		void eraseEmptyWindows();
		void initGLFW();

		//-- Initialization
		SmartWindow init(SmartEVW evw);
		SmartWindow init(SmartEVW evw, const char* name);
		SmartWindow init(SmartEVW evw, int width, int height);
		SmartWindow init(SmartEVW evw, int width, int height, const char* name);
		SmartWindow init(SmartEVW evw, SmartWindow winc);

		//-- Main
		void update(SmartWindow winc);
		void terminate(SmartWindow winc);
	}
}
#endif