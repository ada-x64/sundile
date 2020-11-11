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
#include <filesystem>
#include <any>
namespace fs = std::filesystem;

//std assert
#include <assert.h>

//std data structures
#include <string>
#include <vector>
#include <map>
#include <functional>

//c++20
#ifdef WIN32
#ifndef __cpp_lib_concepts
#define __cpp_lib_concepts
#endif
#endif
#include <concepts>


//stb
#include <stb_image.h>

//algo
#include <algorithm>
#include <random>

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
#ifdef APPLE
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

//entt
#include "../ext/entt.hpp"
//glfw
#include <GLFW/glfw3.h>

//imgui
#include <imgui.h>
#include "../ext/imgui/imgui_impl_opengl3.h"
#include "../ext/imgui/imgui_impl_glfw.h"

//sundile features
#include "Math.h"
#include "Macros.h"
#include "Utility.h"
namespace sundile {};
namespace sundile::Components {};
namespace sundile::Systems {};

//environment variables

#endif