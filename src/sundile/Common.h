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

//algo
#include <algorithm>

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
#include <GL/glew.h>
#include <GL/freeglut.h>

//entt
#include "ext/entt.hpp"
//glfw
#include <GLFW/glfw3.h>

//imgui
#include <imgui.h>
#include "ext/imgui_impl_opengl3.h"
#include "ext/imgui_impl_glfw.h"

//Utility
#include "./Utility.h"

//Macros
#define SUNDILE_BEGIN namespace sundile {
#define SUNDILE_END }

//- TODO - create ifndef guards
#define COMPONENT_DEF_BEGIN(name) namespace sundile { namespace Components { struct name/*:component*/ {
#define COMPONENT_DEF_END };}}

#define SYSTEM_DEF_BEGIN(name) namespace sundile { using namespace Components; namespace Systems { namespace name {
#define SYSTEM_DEF_END }}}

#endif