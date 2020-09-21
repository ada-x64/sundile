//--
//-- Renderer.h
//--
#include "../sundile/sundile.h"
#include "Shader.h"

#ifndef RENDERER_H
#define RENDERER_H
BEGIN_COMPONENT(Renderer)
	Shader passthrough;
	bool initialized = false;
	NO_GUI
END_COMPONENT
#endif