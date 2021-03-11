//--
//-- Window.h
//--
#pragma once
#ifndef WINDOW_H
#define WINDOW_H
#include "EventSystem/EventSystem.h"

namespace sundile {
	struct SmartGLFWwindow {
		GLFWwindow* ptr;
		SmartGLFWwindow(GLFWwindow* window) : ptr(window) {};
		~SmartGLFWwindow() {
			glfwDestroyWindow(ptr);
		}
		SmartGLFWwindow operator =(GLFWwindow*& other) {
			ptr = other;
		}
		SmartGLFWwindow operator =(SmartGLFWwindow& other) {
			ptr = other.ptr;
		}
	};
}


BEGIN_COMPONENT(WindowContainer)
	GLFWwindow*		window;
	SmartEVW		evw;
	int				WIDTH = 800;
	int				HEIGHT = 600;
	int				framebufferWidth = 800;
	int				framebufferHeight = 600;
	bool			windowShouldClose = false;
	bool			guiEnabled = false;
	const char* title = "sundile";
	unsigned int id = -1;

	bool operator == (WindowContainer& other) {
		return (this->window == other.window);
	}
	void operator = (WindowContainer& other) {
		this->window = other.window;
	}

	void setTitle(const char* title) {
		this->title = title;
		glfwSetWindowTitle(window,title);
	}
END_COMPONENT

namespace sundile {
	typedef std::shared_ptr<WindowContainer> SmartWindow;
}

BEGIN_SYSTEM(WindowSystem)
	inline SmartWindow emptyWindow = std::make_shared<WindowContainer>();
	inline SmartWindow currentWindow;
	inline SmartEVW	currentEVW;
	inline bool	GLFWinitialized = false;
	inline bool	GLEWinitialized = false;
	inline bool termination_called = false;
	inline std::vector<SmartWindow> windows	= std::vector<SmartWindow>();

	//-- Helpers
	SmartWindow getSmartWindow(GLFWwindow* w);
	SmartWindow getSmartWindow(const unsigned int id);
	void eraseEmptyWindows();
	void initGLFW();
	Vec2 getWindowSize(SmartWindow winc);

	//-- Initialization
	SmartWindow init(SmartEVW evw);
	SmartWindow init(SmartEVW evw, const char* name);
	SmartWindow init(SmartEVW evw, int width, int height);
	SmartWindow init(SmartEVW evw, int width, int height, const char* name);
	SmartWindow init(SmartEVW evw, SmartWindow winc);
	SmartWindow initFullscreen(SmartEVW evw, const char* name = "sundile");
	SmartWindow initWindowedFullscreen(SmartEVW evw, const char* name = "sundile");

	//-- Main
	void update(SmartWindow winc);
	void terminate(SmartWindow winc);
END_SYSTEM

#include "WindowSystemDefinitions.h"

#endif