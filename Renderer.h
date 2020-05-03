//--
//-- Renderer.h
//--

#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include "Window.h"

/*
Renderer singleton.
Note: Renderer MUST be defined AFTER Window, or it will cause OpenGL to crash.
*/
namespace sundile {
	namespace Renderer
	{
		static Shader passthrough;

		void init();
		void render();
	}
}

#endif