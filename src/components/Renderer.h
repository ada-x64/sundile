//--
//-- Renderer.h
//--
#include "../sundile/sundile.h"

COMPONENT_DEF_BEGIN(Renderer)
Shader passthrough = ShaderSystem::init("assets/shaders/passthrough.vert", "assets/shaders/passthrough.frag");
COMPONENT_DEF_END