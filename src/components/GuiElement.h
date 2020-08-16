//--
//-- GuiElement.h
//--
#pragma once
#include "../sundile/sundile.h"
using namespace sundile;

COMPONENT_DEF_BEGIN(guiElement)

std::function<void()> renderFunc;

COMPONENT_DEF_END