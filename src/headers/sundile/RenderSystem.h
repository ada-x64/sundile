//--
//-- Renderer.h
//--

#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include "GameSystem.h"
#include "Shader.h"
#include "Model.h"

namespace sundile {

	struct Renderer {
		const char* glsl_version = "#version 130";
		Shader passthrough = ShaderSystem::init("assets/shaders/passthrough.vert", "assets/shaders/passthrough.frag");
		SmartEVW evw;
		Game* game;
	};

	namespace RenderSystem
	{
		Renderer init(SmartEVW evw, Game& game);
		Renderer init(Renderer& rend);
		Renderer render(Renderer& rend);
		void terminate(Renderer& rend);
	}
}

#endif