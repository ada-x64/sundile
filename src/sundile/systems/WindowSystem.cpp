#include "WindowSystem.h"
namespace sundile {
	namespace WindowSystem {

		namespace /*Callback functions*/ {

			void errorCallback(int error, const char* description) {
				fprintf(stderr, "GLFW Error: %s\n", description);
			}
			void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) {
				WindowInputEvent ev;
				ev.window = w;
				ev.key = key;
				ev.scancode = scancode;
				ev.action = action;
				ev.mods = mods;
				currentevw->dispatcher.trigger<WindowInputEvent>( ev );
			}
			void mouseBtnCallback(GLFWwindow* w, int button, int actions, int mods) {
				WindowInputEvent ev;
				ev.window = w;
				ev.key = button;
				ev.scancode = GLFW_KEY_UNKNOWN;
				ev.action = actions;
				ev.mods = mods;
				currentevw->dispatcher.trigger<WindowInputEvent>(ev);
			}
			void cursorPosCallback(GLFWwindow* w, double x, double y) {
				TypedWindowEvent<double> ev;
				ev.window = w;
				ev.vals = std::vector<double>{ x,y };
				currentevw->dispatcher.trigger<TypedWindowEvent<double>>(ev);
			}
			void framebufferSizeCallback(GLFWwindow* w, int width, int height) {
				glViewport(0, 0, width, height);
				//TypedWindowEvent<int> wev{  w, {width, height} };
				//currentevw->getEvent(wev);
			}
			void windowCloseCallback(GLFWwindow* w) {
				WindowTerminateEvent ev;
				ev.window = w;
				currentevw->dispatcher.enqueue<WindowTerminateEvent>(ev);

			}
			//etc.
			//TODO: Fill this out as needed

			void setCallbacks(GLFWwindow* win) {
				glfwSetKeyCallback(win, keyCallback);
				glfwSetMouseButtonCallback(win, mouseBtnCallback);
				glfwSetCursorPosCallback(win, cursorPosCallback);
				glfwSetFramebufferSizeCallback(win, framebufferSizeCallback);
				glfwSetWindowCloseCallback(win, windowCloseCallback);
			}

		}
		//-----------------------------------------------------------------------------------------

		namespace /* Event catchers */ {

			void eventSystemInit(const initEvent& ev) {
				for (auto window : windows) {
					ev.evw->dispatcher.trigger<WindowInitEvent>(window.get()->window.get());
				}
			}

			void catchTerminateEvent(const WindowTerminateEvent& wev) {
				terminate(getSmartWindow(wev.window));
			}

			void preupdate(SmartWindow& winc) {
				auto currentwindow = winc->window.get();
				auto currentevw = winc->evw;

				glfwMakeContextCurrent(currentwindow);
				glfwPollEvents();

				if (winc->windowShouldClose) {
					WindowEvent ev;
					ev.window = winc->window.get();
					currentevw->dispatcher.enqueue<WindowEvent>(ev);
					glfwSetWindowShouldClose(currentwindow, GLFW_TRUE);
				}
			}

			void postupdate(SmartWindow& winc) {
				auto currentwindow = winc->window.get();
				glfwMakeContextCurrent(currentwindow);
				glfwSwapBuffers(currentwindow);
			}

			void updateAll(const preStepEvent& ev) {
				for (auto window : windows) {
					preupdate(window);
				}

				for (auto window : windows) {
					window->evw->dispatcher.update<WindowEvent>();
					window->evw->dispatcher.update<WindowInputEvent>();
					window->evw->dispatcher.update<TypedWindowEvent<double>>();
				}

				for (auto window : windows) {
					postupdate(window);
				}
			}

			void postStep(const postStepEvent& ev) {
				for (auto window : windows) {
					window->evw->dispatcher.update<WindowTerminateEvent>();
				}
			}

			void setEventCallbacks(SmartEVW evw) {
				evw->dispatcher.sink<WindowTerminateEvent>().connect<catchTerminateEvent>();
				evw->dispatcher.sink<preStepEvent>().connect<updateAll>();
				evw->dispatcher.sink<postStepEvent>().connect<postStep>();
				evw->dispatcher.sink<initEvent>().connect<eventSystemInit>();
			}
		}
		//-----------------------------------------------------------------------------------------

		//--
		//-- Helpers
		//--
		// Find a SmartWindow by its pointer.
		SmartWindow getSmartWindow(GLFWwindow* w) {
			for (SmartWindow winc : windows) {
				if (winc->window.get() == w) {
					return winc;
				}
			}
			return 0;
		}

		Vec2 getWindowSize(SmartWindow winc) {
			int wwidth = 0, wheight = 0;
			glfwGetWindowSize(winc->window.get(), &wwidth, &wheight);
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
				return win->window.get() == emptywindow->window.get();
				}), windows.end());

			termination_called = false;

			if (windows.size() == 0)
				EventSystem::run = false;
		}

		// Manually update a window.
		void update(SmartWindow winc) {
			currentevw = winc->evw;

			preupdate(winc);

			//currentevw->dispatcher.enqueue<Event>(DrawEvent{ EventType::generic_draw });
			currentevw->dispatcher.update<WindowEvent>();
			currentevw->dispatcher.update<WindowInputEvent>();
			currentevw->dispatcher.update<TypedWindowEvent<double>>();

			postupdate(winc);
		}

		// Manually terminate a window.
		void terminate(SmartWindow winc) {
			currentevw = winc->evw;
			WindowEvent ev;
			ev.window = winc->window.get();
			winc->window.reset();
			currentwindow = nullptr;
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
			winc->name = title;

			// Init GLFW
			if (!GLFWinitialized) { initGLFW(); }

			// Preliminary
			currentevw = evw;

			// Create a windowed mode window and its OpenGL context
			auto monitor = glfwGetPrimaryMonitor();
			auto mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			auto winptr = glfwCreateWindow(mode->width, mode->height, winc->name, monitor, NULL);
			if (!winptr)
			{
				glfwTerminate();
				exit(EXIT_FAILURE);
			}

			//Set up winc
			winc->window = SmartGLFWwindow(winptr);
			winc->evw = currentevw;

			// Set up the window
			glfwMakeContextCurrent(winc->window.get());
			glfwSwapInterval(1);

			// Init GLEW
			if (!GLEWinitialized) { initGLEW(); }
			if (!GLFWinitialized) { initGLFW(); }

			// Set up event handlers
			setCallbacks(winc->window.get());
			setEventCallbacks(currentevw);

			//Add to vector and return.
			windows.push_back(winc);

			return winc;
		}
		SmartWindow initWindowedFullscreen(SmartEVW evw, const char* title) {

			SmartWindow winc = std::make_shared<WindowContainer>();
			winc->name = title;

			// Init GLFW
			if (!GLFWinitialized) { initGLFW(); }

			// Preliminary
			currentevw = evw;

			// Create a windowed mode window and its OpenGL context
			auto monitor = glfwGetPrimaryMonitor();
			auto mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);


			auto winptr = glfwCreateWindow(mode->width, mode->height, winc->name, NULL, NULL);
			if (!winptr)
			{
				glfwTerminate();
				exit(EXIT_FAILURE);
			}

			//Set up winc
			winc->window = SmartGLFWwindow(winptr);
			winc->evw = currentevw;

			// Set up the window
			glfwMakeContextCurrent(winc->window.get());
			glfwSwapInterval(1);

			// Init GLEW
			if (!GLEWinitialized) { initGLEW(); }

			// Set up event handlers
			setCallbacks(winc->window.get());
			setEventCallbacks(currentevw);

			//Add to vector and return.
			windows.push_back(winc);

			return winc;
		}

		SmartWindow init(SmartEVW evw, const char* name) {
			SmartWindow winc = std::make_shared<WindowContainer>();
			winc->name = name;
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
			winc->name = name;
			return init(evw, winc);
		}

		SmartWindow init(SmartEVW evw, SmartWindow winc) {
			// Init GLFW
			if (!GLFWinitialized) { initGLFW(); }
			glViewport(0, 0, winc->WIDTH, winc->HEIGHT);

			// Preliminary
			currentevw = evw;

			// Create a windowed mode window and its OpenGL context
			auto winptr = glfwCreateWindow(winc->WIDTH, winc->HEIGHT, winc->name, NULL, NULL);
			if (!winptr)
			{
				glfwTerminate();
				exit(EXIT_FAILURE);
			}

			//Set up winc
			winc->window = SmartGLFWwindow(winptr);
			winc->evw = currentevw;

			// Set up the window
			glfwMakeContextCurrent(winc->window.get());
			glfwSwapInterval(1);

			// Init GLEW
			if (!GLEWinitialized) { initGLEW(); }

			// Set up event handlers
			setCallbacks(winc->window.get());
			setEventCallbacks(currentevw);

			//Add to vector and return.
			windows.push_back(winc);

			return winc;
		}

	}
}