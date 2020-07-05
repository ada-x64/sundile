#include "WindowSystem.h"
namespace sundile {
	namespace WindowSystem {

		namespace /*Callback functions*/ {

			void errorCallback(int error, const char* description) {
				fprintf(stderr, "GLFW Error: %s\n", description);
			}
			void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) {
				currentevw->dispatcher.enqueue<WindowInputEvent>({ EventType::key, w,  key, scancode, action, mods  });
			}
			void mouseBtnCallback(GLFWwindow* w, int button, int actions, int mods) {
				currentevw->dispatcher.enqueue<WindowInputEvent>({ EventType::mousebtn, w, button, GLFW_KEY_UNKNOWN, actions, mods });
			}
			void cursorPosCallback(GLFWwindow* w, double x, double y) {
				currentevw->dispatcher.enqueue<TypedWindowEvent<double>>({ EventType::cursorpos, w, {x,y} });
			}
			void framebufferSizeCallback(GLFWwindow* w, int width, int height) {
				glViewport(0, 0, width, height);
				//TypedWindowEvent<int> wev{ EventType::framebufferSize, w, {width, height} };
				//currentevw->getEvent(wev);
			}
			void windowCloseCallback(GLFWwindow* w) {
				currentevw->dispatcher.enqueue<WindowEvent>({ EventType::terminateWindow, w });

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

			void catchWindowEvent(const WindowEvent& wev) {
				if (wev.type == EventType::terminateWindow) {
					terminate(getSmartWindow(wev.window));
				}
			}

			void preupdate(SmartWindow& winc) {
				auto currentwindow = winc->window.get();
				auto currentevw = winc->evw;

				glfwMakeContextCurrent(currentwindow);
				glfwPollEvents();

				if (winc->windowShouldClose) {
					currentevw->dispatcher.enqueue<WindowEvent>({ EventType::terminateWindow, winc->window.get() });
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

					window->evw->dispatcher.enqueue<DrawEvent>({ EventType::generic_draw });
				}

				if (termination_called)
					eraseEmptyWindows();

				for (auto window : windows) {
					postupdate(window);
				}
			}

			void setEventCallbacks(SmartEVW evw) {
				evw->dispatcher.sink<WindowEvent>().connect<catchWindowEvent>();
				evw->dispatcher.sink<preStepEvent>().connect<updateAll>();
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

			currentevw->dispatcher.enqueue<Event>(DrawEvent{ EventType::generic_draw });
			currentevw->dispatcher.update<WindowEvent>();
			currentevw->dispatcher.update<WindowInputEvent>();
			currentevw->dispatcher.update<TypedWindowEvent<double>>();
			
			if (termination_called)
				eraseEmptyWindows();

			postupdate(winc);
		}

		// Manually terminate a window.
		void terminate(SmartWindow winc) {
			currentevw = winc->evw;
			currentevw->dispatcher.enqueue<WindowEvent>({ EventType::window_finishTermination, winc->window.get() });
			winc->window.reset();
			currentwindow = nullptr;
			termination_called = true;
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