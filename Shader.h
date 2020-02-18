#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <cstdio>
#include <GL/glew.h>
#include <gl/freeglut.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
private:
	enum ERRTYPE {
		ERRTYPE_SHADER_VERTEX,
		ERRTYPE_SHADER_FRAGMENT,
		ERRTYPE_PROGRAM,
		ERRTYPE_LENGTH
	};

	void checkError(ERRTYPE type, unsigned int item);

public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

#endif