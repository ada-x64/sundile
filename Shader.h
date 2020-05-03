//--
//-- Shader.h
//-- https://learnopengl.com/Getting-started/Shaders
//-- TODO: Decouple members from data.
//-- This class could be a struct containing only the ID and a class containing only the functions.
//--
#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "Common.h"
#include "System.h"
namespace sundile {

	//--
	//-- Shader Struct
	//--
	struct Shader {
		//-- Data
		unsigned int ID = 0;

		//-- Constructors
		Shader() {};
		Shader(unsigned int ID) {
			this->ID = ID;
		}
		//-- Operators
		bool operator == (Shader other) {
			return (this->ID == other.ID);
		}
		bool operator != (Shader other) {
			return (this->ID != other.ID);
		}
	};

	//-- Data
	static std::vector<Shader> ShadersInUse = std::vector<Shader>();

	//--
	//-- ShaderSystem
	//--
	namespace ShaderSystem
	{

		//-- Error Checking
		void checkError(const char* type, unsigned int item);

		//-- Create and Initialize
		void init(Shader& s, const char* vertexPath, const char* fragmentPath);
		Shader init(const char* vertexPath, const char* fragmentPath);

		//-- Use
		void use(Shader& s);

		//-- Utility Functions
		void setBool(Shader s, const std::string& name, bool value);
		void setInt(Shader s, const std::string& name, int value);
		void setFloat(Shader s, const std::string& name, float value);
		void setVec2(Shader s, const std::string& name, const glm::vec2& value);
		void setVec2(Shader s, const std::string& name, float x, float y);
		void setVec3(Shader s, const std::string& name, const glm::vec3& value);
		void setVec3(Shader s, const std::string& name, float x, float y, float z);
		void setVec4(Shader s, const std::string& name, const glm::vec4& value);
		void setVec4(Shader s, const std::string& name, float x, float y, float z, float w);
		void setMat2(Shader s, const std::string& name, const glm::mat2& mat);
		void setMat3(Shader s, const std::string& name, const glm::mat3& mat);
		void setMat4(Shader s, const std::string& name, const glm::mat4& mat);
	};
};
#endif