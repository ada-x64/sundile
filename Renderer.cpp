#include "Renderer.h"

void Renderer::init() {
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


	//General settings
	glEnable(GL_DEPTH_TEST);
	stbi_set_flip_vertically_on_load(true);


	/* Objects */
	//Generate
	glGenBuffers(vbo_num, VBOs);
	glGenBuffers(ebo_num, EBOs);
	glGenVertexArrays(vao_num, VAOs);

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

	//Bind individual vertex arrays / buffers
	glBindVertexArray(VAOs[0]);

	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(0*sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
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
	for (unsigned int i = 0; i < 1; i++) {
		//Do this for each model
		mat_model = glm::rotate(mat_model, glm::radians(sin(float(glfwGetTime()))), glm::vec3(0.f, 1.f, 0.f));
		int loc_model = glGetUniformLocation(sp_passthrough->ID, "model");
		glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(mat_model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindVertexArray(0);
}