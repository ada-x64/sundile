#include "Window.h"
namespace sundile {
	//--
	//-- Helpers
	//--
	static void error_callback(int error, const char* description) {
		fprintf(stderr, "Error: %s\n", description);
	}

	void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	//--
	//-- Events
	//--
	void Window::handleKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Systems::dispatcher.enqueue<Systems::keyEvent>({ key, scancode, action, mods });
	}

	//--
	//--Initialize GLFW
	//--
	void Window::init() {
		//Preliminary
		glfwSetErrorCallback(error_callback);

		/* Initialize the library */
		if (!glfwInit())
			exit(EXIT_FAILURE);

		//Set Minimum OpenGL Version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		//Apple compability
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		//Set up the window
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
		glfwSwapInterval(1);

		//Set up event handlers
		glfwSetKeyCallback(window, handleKeyInput);

		//Start up GLEW
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
	}

	//--
	//-- Update in the main loop
	//--
	void Window::update() {
		glfwPollEvents();
		Systems::dispatcher.update();
		if (Systems::windowShouldClose) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		if (glfwWindowShouldClose(window)) {
			Systems::windowShouldClose = true;
		}
		glfwSwapBuffers(window);
	}
}