//--
//-- Renderer.h
//--

#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include "Shader.h"
#include "Model.h"
#include "EventWrapper.h"
#include "AllComponents.h"

namespace sundile {

	struct Renderer {
		const char* glsl_version = "#version 130";
		Shader passthrough = ShaderSystem::init("assets/shaders/passthrough.vert", "assets/shaders/passthrough.frag");
		SmartEVW evw;
		SmartRegistry registry;
	};

	namespace RenderSystem
	{
		Renderer init(SmartEVW evw, SmartRegistry registry);
		Renderer init(Renderer& rend);
		Renderer render(Renderer& rend);
		void terminate(Renderer& rend);
	}
}

#endif