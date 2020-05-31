#include "./headers/sundile/WindowSystem.h"
namespace sundile {
	namespace WindowSystem {

		namespace /*Callback functions*/ {

			void errorCallback(int error, const char* description) {
				fprintf(stderr, "GLFW Error: %s\n", description);
			}
			void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) {
				currentevw->dispatcher.enqueue<InputEvent>({ EventType::key, w,  key, scancode, action, mods  });
			}
			void mouseBtnCallback(GLFWwindow* w, int button, int actions, int mods) {
				currentevw->dispatcher.enqueue<InputEvent>({ EventType::mousebtn, w, button, GLFW_KEY_UNKNOWN, actions, mods });
			}
			void cursorPosCallback(GLFWwindow* w, double x, double y) {
				currentevw->dispatcher.enqueue<TypedWindowEvent<double>>({ EventType::cursorpos, w, {x,y} });
			}
			void framebufferSizeCallback(GLFWwindow* w, int width, int height) {
				glViewport(0, 0, width, height);
				//TypedWindowEvent<int> wev{ EventType::framebufferSize, w, {width, height} };
				//currentevw->getEvent(wev);
			}
			//etc.
			//TODO: Fill this out as needed

			void setCallbacks(GLFWwindow* win) {
				glfwSetKeyCallback(win, keyCallback);
				glfwSetMouseButtonCallback(win, mouseBtnCallback);
				glfwSetCursorPosCallback(win, cursorPosCallback);
				glfwSetFramebufferSizeCallback(win, framebufferSizeCallback);
			}

		}
		//-----------------------------------------------------------------------------------------

		//--
		//-- Initialize GLFW
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

		//--
		//-- Initialize a window
		//--
		SmartWindow init(SmartEVW evw) {
			SmartWindow winc = std::make_shared<WindowContainer>();
			return init(evw, winc);
		}

		SmartWindow init(SmartEVW evw, SmartWindow winc) {
			// Init GLFW
			if (!GLFWinitialized) { initGLFW(); }

			// Preliminary
			currentevw = evw;

			// Create a windowed mode window and its OpenGL context
			auto winptr = glfwCreateWindow(winc->WIDTH, winc->HEIGHT, "Hello World", NULL, NULL);
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

			//Add to vector and return.
			windows.push_back(winc);

			int i = 0;
			for (auto window : windows) {
				window->vecIndex = i++;
			}

			return winc;
		}

		//--
		//-- Update in the main loop
		//--
		SmartWindow update(SmartWindow winc) {
			auto currentwindow = winc->window.get();
			auto currentevw = winc->evw;

			glfwMakeContextCurrent(currentwindow); //TODO: breaks here
			glfwPollEvents();
			currentevw->dispatcher.enqueue<Event>(DrawEvent {EventType::generic_draw});
			currentevw->dispatcher.update<WindowEvent>();
			currentevw->dispatcher.update<InputEvent>();
			currentevw->dispatcher.update<TypedWindowEvent<double>>();

			if (winc->windowShouldClose || glfwWindowShouldClose(currentwindow)) {
				glfwSetWindowShouldClose(currentwindow, GLFW_TRUE);
			}

			glfwSwapBuffers(currentwindow);

			return winc;
		}

		void updateAll() {
			for (auto window : windows) {
				update(window);
			}
		}

		//--
		//-- Terminate
		//-- 
		void terminate(SmartWindow winc) {
			currentevw = winc->evw;
			currentevw->dispatcher.enqueue<WindowEvent>(WindowEvent { EventType::terminateWindow, winc->window.get() });
			// should probably delete the window *after* all relevant calls are made.
			// entt doesn't guarantee order when using dispatcher. will need to do this manually.
		}

		void finishTermination(SmartWindow winc) {
			currentevw = winc->evw;
			winc->window.reset();
			int i = 0;


			for (std::vector<SmartWindow>::iterator it = windows.begin(); it != windows.end(); ++it) {
				if (*it == winc) {
					windows.erase(it);
				}
			}
		}

	}
}