#include "Renderer.h"

//Local Helpers
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

//Loaders
void Renderer::loadShaders() {
	shaders.emplace("passthrough", new Shader("./shaders/passthrough.vert", "./shaders/passthrough.frag"));
	//Shader* sp_orange = new Shader("./shaders/passthrough.vert", "./shaders/orange.frag");
	//Shader* sp_passthrough = new Shader("./shaders/passthrough.vert", "./shaders/passthrough.frag");
}

void Renderer::loadModels() {
	//Temporary manual model code
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f
	};

	//unsigned int indices[] = {
	//	0, 1, 3, //front
	//	1, 2, 3, 
	//	3, 7, 6, //left
	//	3, 2, 6,
	//	7, 4, 3, //top
	//	3, 4, 0,
	//	0, 4, 1, //right
	//	1, 5, 4,
	//	4, 7, 6, //back
	//	6, 4, 5,
	//	5, 6, 2, //bottom
	//	2, 5, 1,
	//};

	//Generate Buffers and Arrays
	glGenBuffers(vbo_num, VBOs);
	glGenBuffers(ebo_num, EBOs);
	glGenVertexArrays(vao_num, VAOs);

	//Bind
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind individual vertex arrays / buffers
	glBindVertexArray(VAOs[0]);
}

void Renderer::loadTextures() {
	//Texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	//Load texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, numChannels;
	unsigned char* data = stbi_load("./textures/wall.jpg", &width, &height, &numChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture!\n";
	}
	stbi_image_free(data);
}

//Class functions
Renderer::Renderer() {

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

	//Start up GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
}

void Renderer::init() {
	//Set up
	glEnable(GL_DEPTH_TEST);
	stbi_set_flip_vertically_on_load(true);

	//Load Models
	loadModels();

	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/

	//Load Shaders
	loadShaders();

	//Load Textures
	loadTextures();


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Reset binds
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::render() {
	//Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set up shader
	Shader* sp_passthrough = shaders["passthrough"];
	sp_passthrough->use();

	//Apply camera
	int loc_view = glGetUniformLocation(sp_passthrough->ID, "view");
	glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(mat_view));

	int loc_proj = glGetUniformLocation(sp_passthrough->ID, "projection");
	glUniformMatrix4fv(loc_proj, 1, GL_FALSE, glm::value_ptr(mat_proj));

	//Render
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAOs[0]);
	mat_model = glm::rotate(mat_model, glm::radians(sin(float(glfwGetTime()))), glm::vec3(0.f, 1.f, 0.f));
	int loc_model = glGetUniformLocation(sp_passthrough->ID, "model");
	glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(mat_model));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);

	//Swap and poll IO
	glfwSwapBuffers(window);
	glfwPollEvents();
}