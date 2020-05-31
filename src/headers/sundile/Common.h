//--
//-- Common.h
//-- -- Includes all necessary libraries. Circumvents circular #include's.
//--

#pragma once
#ifndef COMMON_H
#define COMMON_H

//std streams
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>

//std assert
#include <assert.h>

//std data structures
#include <string>
#include <vector>
#include <map>
#include <functional>

//stb
#include <stb_image.h>

//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//maths
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

//gl
#include <gl/glew.h>
#include <gl/freeglut.h>

//entt
#include "entt.hpp"

//glfw
#include <GLFW/glfw3.h>

//imgui
#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_opengl3.h"
#include "../../ext/imgui/imgui_impl_glfw.h"

#endif