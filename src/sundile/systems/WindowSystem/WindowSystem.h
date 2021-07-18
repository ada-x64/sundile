#include "WindowTypes.h"
SYSTEM(WindowSystem)

	namespace /*Callback functions*/ {

		void errorCallback(int error, const char* description) {
			fprintf(stderr, "GLFW Error: %s\n", description);
		}
		void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) {
			InputEvent<SmartWindow> ev{ getSmartWindow(w) };
			InputSystem::interpretInput(ev.member->input, key, scancode, action, mods);
			currentEVW->dispatcher.trigger<InputEvent<SmartWindow>>(ev);
		}
		void mouseBtnCallback(GLFWwindow* w, int button, int actions, int mods) {
			InputEvent<SmartWindow> ev{ getSmartWindow(w) };
			InputSystem::interpretInput(ev.member->input, button, GLFW_KEY_UNKNOWN, actions, mods);
			currentEVW->dispatcher.trigger<InputEvent<SmartWindow>>(ev);
		}
		void cursorPosCallback(GLFWwindow* w, double x, double y) {
			InputEvent<SmartWindow> ev{ getSmartWindow(w) };
			auto& window = WindowSystem::currentWindow->window;
			if (window == nullptr) return;
			int* width = new int;
			int* height = new int;
			double* xpos = new double;
			double* ypos = new double;
			glfwGetFramebufferSize(window, width, height);
			glfwGetCursorPos(window, xpos, ypos);
			Vec2 cursorpos = Vec2((*xpos) / (*width), (*ypos) / (*height)); //normalized :)
			delete width;
			delete height;
			delete xpos;
			delete ypos;
			InputSystem::interpretCursorPos(ev.member->input, cursorpos);
			currentEVW->dispatcher.trigger<InputEvent<SmartWindow>>(ev);
		}
		void framebufferSizeCallback(GLFWwindow* w, int width, int height) {
			auto window = getSmartWindow(w);
			window->framebufferHeight = height;
			window->framebufferWidth = width;
			glViewport(0, 0, width, height);
		}
		void windowSizeCallback(GLFWwindow* w, int width, int height) {
			auto window = getSmartWindow(w);
			window->WIDTH = width;
			window->HEIGHT = height;
		}
		void windowCloseCallback(GLFWwindow* w) {
			DestroyEvent<SmartWindow> ev;
			ev.member = getSmartWindow(w);
			currentEVW->dispatcher.enqueue<DestroyEvent<SmartWindow>>(ev);

		}
		//etc.
		//\todo: Fill this out as needed

		void setCallbacks(GLFWwindow* win) {
			glfwSetKeyCallback(win, keyCallback);
			glfwSetMouseButtonCallback(win, mouseBtnCallback);
			glfwSetCursorPosCallback(win, cursorPosCallback);
			//glfwSetFramebufferSizeCallback(win, framebufferSizeCallback);
			glfwSetWindowSizeCallback(win, windowSizeCallback);
			glfwSetWindowCloseCallback(win, windowCloseCallback);
		}

	}
	//-----------------------------------------------------------------------------------------

	namespace /* Event catchers */ {

		void closeWindow(SmartWindow& winc) {
			Event<SmartWindow> ev;
			ev.member = winc;
			currentEVW->dispatcher.enqueue<Event<SmartWindow>>(ev);
			glfwSetWindowShouldClose(winc->window, GLFW_TRUE);
			removeErase(windows, winc);
			eraseEmptyWindows();
			glfwDestroyWindow(winc->window);
		}

		void eventSystemInit(const InitEvent<SmartEVW>& ev) {
			for (auto window : windows) {
				InitEvent<SmartWindow> wev{ window};
				ev.member->dispatcher.trigger<InitEvent<SmartWindow>>(wev);
			}
		}

		void catchTerminateEvent(DestroyEvent<SmartWindow>& wev) {
			closeWindow(wev.member);
		}

		void preupdate(SmartWindow& winc) {

			if (windows.size() == 0) {
				DestroyEvent<SmartWindow> tev;
				tev.member = winc;
				winc->evw->dispatcher.trigger<DestroyEvent<SmartWindow>>(tev);
			}
			currentWindow = winc;
			auto window = winc->window;

			if (winc->windowShouldClose) {
				closeWindow(winc);
			}
			else if (winc->window) {
				glfwMakeContextCurrent(window);
				glfwPollEvents();
			}
		}

		void postupdate(SmartWindow& winc) {
			if (winc->windowShouldClose) {
				closeWindow(winc);
			}
			else if (winc->window) {
				auto window = winc->window;
				glfwMakeContextCurrent(window);
				glfwSwapBuffers(window);
			}
		}

		void updateAll(const PreStepEvent<SmartEVW>& ev) {
			for (auto window : windows) {
				preupdate(window);
			}

			for (auto window : windows) {
				window->evw->dispatcher.update<Event<SmartWindow>>();
				window->evw->dispatcher.update<InputEvent<SmartWindow>>();
			}

			for (auto window : windows) {
				postupdate(window);
			}
		}

		void postStep(const PostStepEvent<SmartEVW>& ev) {
			for (auto window : windows) {
				window->evw->dispatcher.update<DestroyEvent<SmartWindow>>();
			}
		}

		void setEventCallbacks(SmartEVW evw) {
			evw->dispatcher.sink<DestroyEvent<SmartWindow>>().connect<catchTerminateEvent>();
			evw->dispatcher.sink<PreStepEvent<SmartEVW>>().connect<updateAll>();
			evw->dispatcher.sink<PostStepEvent<SmartEVW>>().connect<postStep>();
			evw->dispatcher.sink<InitEvent<SmartEVW>>().connect<eventSystemInit>();
		}
	}
	//-----------------------------------------------------------------------------------------

	//--
	//-- Helpers
	//--
	// Find a SmartWindow by its pointer.
	SmartWindow getSmartWindow(GLFWwindow* w) {
		for (SmartWindow winc : windows) {
			if (winc->window == w) {
				return winc;
			}
		}
		return 0;
	}
	// Find a SmartWindow by its id.
	SmartWindow getSmartWindow(const unsigned int id) {
		for (SmartWindow winc : windows) {
			if (winc->id == id) {
				return winc;
			}
		}
		return 0;
	}

	Vec2 getWindowSize(SmartWindow winc) {
		int wwidth = 0, wheight = 0;
		glfwGetWindowSize(winc->window, &wwidth, &wheight);
		if (wwidth && wheight) {
			winc->HEIGHT = wwidth;
			winc->WIDTH = wheight;
		}
		float width = static_cast<float>(winc->WIDTH);
		float height = static_cast<float>(winc->HEIGHT);
		return Vec2{ width, height };

	}

	// Remove-erase all empty windows, end the process if no windows are open.
	void eraseEmptyWindows() {
		windows.erase(std::remove_if(windows.begin(), windows.end(), [=](SmartWindow win) {
			return win == emptyWindow;
			}), windows.end());

		termination_called = false;

		if (windows.size() == 0)
			EventSystem::run = false;
	}

	// Manually update a window.
	void update(SmartWindow winc) {
		currentEVW = winc->evw;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		preupdate(winc);

		currentEVW->dispatcher.update<Event<SmartWindow>>();
		currentEVW->dispatcher.update<InputEvent<SmartWindow>>();

		postupdate(winc);
	}

	// Manually terminate a window.
	void terminate(SmartWindow winc) {
		//\TODO: this isn't actually destroying the window...?
		currentEVW = winc->evw;
		Event<SmartWindow> ev{ winc };
		glfwDestroyWindow(winc->window);
		winc->window = nullptr;
		currentWindow = nullptr;
		eraseEmptyWindows();
	}

	//-----------------------------------------------------------------------------------------

	//--
	//-- Initialize
	//--
	void initGLFW() {

		//Set callback
		glfwSetErrorCallback(errorCallback);

		//Init GLFW
		if (!glfwInit())
			exit(EXIT_FAILURE);

		//Set Minimum OpenGL Version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		//Apple compability
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


		//Don't do this again
		GLFWinitialized = true;
	}

	void initGLEW() {
		//Start up GLEW
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		}

		GLEWinitialized = true;
	}

	SmartWindow init(SmartEVW evw) {
		SmartWindow winc = std::make_shared<WindowContainer>();
		return init(evw, winc);
	}

	SmartWindow initFullscreen(SmartEVW evw, const char* title) {

		SmartWindow winc = std::make_shared<WindowContainer>();
		winc->title = title;

		// Init GLFW
		if (!GLFWinitialized) { initGLFW(); }

		// Preliminary
		currentEVW = evw;

		// Create a windowed mode window and its OpenGL context
		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		auto winptr = glfwCreateWindow(mode->width, mode->height, winc->title, monitor, NULL);
		if (!winptr)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		//Set up winc
		winc->window = winptr;
		winc->evw = currentEVW;
		winc->id = rand();

		// Set up the window
		glfwMakeContextCurrent(winc->window);
		glfwSwapInterval(1);

		// Init GLEW
		if (!GLEWinitialized) { initGLEW(); }
		if (!GLFWinitialized) { initGLFW(); }

		// Set up event handlers
		setCallbacks(winc->window);
		setEventCallbacks(currentEVW);

		//Add to vector and return.
		windows.push_back(winc);

		return winc;
	}
	SmartWindow initWindowedFullscreen(SmartEVW evw, const char* title) {

		SmartWindow winc = std::make_shared<WindowContainer>();
		winc->title = title;

		// Init GLFW
		if (!GLFWinitialized) { initGLFW(); }

		// Preliminary
		currentEVW = evw;

		// Create a windowed mode window and its OpenGL context
		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);


		auto winptr = glfwCreateWindow(1, 1, winc->title, NULL, NULL);
		if (!winptr)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		//Set up winc
		winc->window = winptr;
		winc->evw = currentEVW;
		winc->WIDTH = mode->width;
		winc->HEIGHT = mode->height;
		winc->id = rand();

		// Set up the window
		glfwMakeContextCurrent(winc->window);
		glfwSwapInterval(1);

		// Init GLEW
		if (!GLEWinitialized) { initGLEW(); }

		// Set up event handlers
		setCallbacks(winc->window);
		setEventCallbacks(currentEVW);

		//Add to vector and return.
		windows.push_back(winc);

		return winc;
	}

	SmartWindow init(SmartEVW evw, const char* name) {
		SmartWindow winc = std::make_shared<WindowContainer>();
		winc->title = name;
		return init(evw, winc);
	}

	SmartWindow init(SmartEVW evw, int width, int height) {
		SmartWindow winc = std::make_shared<WindowContainer>();
		winc->WIDTH = width;
		winc->HEIGHT = height;
		return init(evw, winc);
	}

	SmartWindow init(SmartEVW evw, int width, int height, const char* name) {
		SmartWindow winc = std::make_shared<WindowContainer>();
		winc->WIDTH = width;
		winc->HEIGHT = height;
		winc->title = name;
		return init(evw, winc);
	}

	SmartWindow init(SmartEVW evw, SmartWindow winc) {
		// Init GLFW
		if (!GLFWinitialized) { initGLFW(); }
		glViewport(0, 0, winc->WIDTH, winc->HEIGHT);

		// Preliminary
		currentEVW = evw;

		// Create a windowed mode window and its OpenGL context
		auto winptr = glfwCreateWindow(winc->WIDTH, winc->HEIGHT, winc->title, NULL, NULL);
		if (!winptr)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		//Set up winc
		winc->window = winptr;
		winc->evw = currentEVW;
		winc->id = rand();

		// Set up the window
		glfwMakeContextCurrent(winc->window);
		glfwSwapInterval(1);

		// Init GLEW
		if (!GLEWinitialized) { initGLEW(); }

		// Set up event handlers
		setCallbacks(winc->window);
		setEventCallbacks(currentEVW);

		//Add to vector and return.
		windows.push_back(winc);

		return winc;
	}

END_SYSTEM