//--
//-- GuiSystem.h
//--

#pragma once

#ifndef GUI_H
#define GUI_H
#include "Common.h"
#include "EventWrapper.h"
namespace sundile {
	namespace GuiSystem {
		extern ImGuiIO& io;
		extern ImGuiContext *ctx;
		extern const char* glsl_version;

		void Init(GLFWwindow *window, const char *glsl_version);
		void Render();
		void Terminate();
	}
}

#endif