//--
//-- Shader.h
//-- https://learnopengl.com/Getting-started/Shaders
//--
#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "Common.h"
namespace sundile {

	//--
	//-- Struct
	//--
	typedef unsigned int Shader;

	//-- Registry
	extern std::vector<Shader> ShaderRegistry;

	//--
	//-- System
	//--
	namespace ShaderSystem
	{

		//-- Error Checking
		void checkError(const char* type, unsigned int item);

		//-- Create and Initialize
		Shader init(Shader& s, fs::path vertexPath, fs::path fragmentPath);
		Shader init(fs::path vertexPath, fs::path fragmentPath);

		//-- Use
		void use(Shader s);

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