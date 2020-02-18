#include "Renderer.h"

void Renderer::init() {
	//All vertices go in this array
	float tri[] = {
		 0.0f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f,
	};

	/* Objects */
	//Generate
	glGenBuffers(vbo_num, VBOs);
	glGenVertexArrays(vao_num, VAOs);

	//Bind individual vertex arrays / buffers
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Reset binds
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::render() {
	sp_yellow->use();
	glBindVertexArray(VAOs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
}