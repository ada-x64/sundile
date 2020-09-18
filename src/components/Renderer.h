//--
//-- Renderer.h
//--
#include "../sundile/sundile.h"
#include "Shader.h"

#ifndef RENDERER_H
#define RENDERER_H
BEGIN_COMPONENT(Renderer)
	Shader passthrough;
	NO_GUI
END_COMPONENT
#endif