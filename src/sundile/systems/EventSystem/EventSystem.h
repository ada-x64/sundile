#pragma once
#ifndef SUNDILE_ENTT_H
#define SUNDILE_ENTT_H

#include "../../globals/Common.h"
#include "EventTypes.h"

namespace sundile::EventSystem {
	inline bool run = true;
	inline std::vector<SmartEVW> EVWs;

	SmartEVW create();

	void init(SmartEVW evw);
	void initAll();
	void updateAll();
	void terminate(SmartEVW evw);
	void terminateAll();
}

#include "EventUtilities.h"
#endif