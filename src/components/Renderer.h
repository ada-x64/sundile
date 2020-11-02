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
	Vec2 pos;
END_COMPONENT
#endif