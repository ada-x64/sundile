#include "Renderer.h"

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

int main(void)
{
	//Preliminary
	glfwSetErrorCallback(error_callback);
	GLFWwindow* window;
	int WIDTH = 640, HEIGHT = 480;

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

	//Start up GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	//Define and initialize renderer
	Renderer R;
	R.init();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//Process input
		processInput(window);

		//Render
		R.render();

		//Swap and poll IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}