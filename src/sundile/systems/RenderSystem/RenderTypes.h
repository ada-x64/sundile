#ifndef RENDER_TYPES
#define RENDER_TYPES

//TODO: make these COMPONNENTs
namespace sundile {
	//types
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};
	struct Texture {
		unsigned int id;
		std::string type; //"specular" or "diffuse"
		std::string path;
	};
	struct Mesh {
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) : vertices(vertices), indices(indices), textures(textures) {};
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		bool is_wireframe = false;
		bool is_visible = true;
		glm::mat4 transform = glm::mat4(1.f);
		unsigned int VAO, VBO, EBO;
	};

	struct Model {
		std::vector<Texture> textures_loaded;
		std::string directory;
		std::vector<Mesh> meshes;
		bool is_visible = true;
		bool is_wireframe = false;
		glm::mat4 transform = glm::mat4(1.f);
	};

	struct Renderer {
		Shader defaultShader;
		bool initialized = false;
		Vec2 pos;
		Vec2 size;
	};
}

COMPONENT(Camera)
	//-- Matrices
	Vec3 pos = Vec3(1.f, 1.f, 1.f);
	Vec3 front = glm::normalize(glm::vec3(-1.f, -1.f, -1.f));
	Vec3 dir = Vec3(0.f, 5.f * glm::quarter_pi<float>(), 0.f);
	Vec3 spd = Vec3(0.f);
	float fric = 0.01f;
	float maxspd = 0.5f;
	Vec2 cursorpos;
	Vec2 cursorpos_prev;
	bool locked = false;
	Vec2 size = { 1920,1080 };

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), size.x / size.y, 0.1f, 100.0f);
	glm::mat4 T =
	glm::rotate(
		glm::translate(glm::mat4(1.f), glm::vec3(-1.f, -1.f, -3.f)),
		dir.y, glm::vec3(0, 1, 0));

	SERIALIZE(Camera, pos, size, T);
END_COMPONENT


#endif