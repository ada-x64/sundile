#include "Shader.h"
void Shader::checkError(ERRTYPE type, unsigned int item) {

	int success;
	char infoLog[512];

	switch (type) {
	case(ERRTYPE_SHADER_VERTEX):
	case(ERRTYPE_SHADER_FRAGMENT):
		glGetShaderiv(item, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(item, 512, NULL, infoLog);
			if (type == ERRTYPE_SHADER_VERTEX) {
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
			else {
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}
		break;
	case(ERRTYPE_PROGRAM):
		glGetProgramiv(item, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(item, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		break;
	}
}


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile(vertexPath);
	std::ifstream fShaderFile(fragmentPath);

	// ensure ifstream objects can throw exceptions:
	try
	{
		// open files
		if (!vShaderFile) {
			std::cout << "ERROR::SHADER::VERTEX::FILE_NOT_SUCCESSFULLY_READ\n";
		}
		if (!fShaderFile) {
			std::cout << "ERROR::SHADER::FRAGMENT::FILE_NOT_SUCCESSFULLY_READ\n";
		}
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	GLuint vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkError(ERRTYPE_SHADER_VERTEX, vertex);

	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// similiar for Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkError(ERRTYPE_SHADER_FRAGMENT, fragment);

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkError(ERRTYPE_PROGRAM, ID);

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
Shader::~Shader() {

}

void Shader::use() {
	glUseProgram(ID);
}
void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}