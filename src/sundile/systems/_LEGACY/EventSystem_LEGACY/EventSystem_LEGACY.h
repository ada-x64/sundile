#pragma once
#ifndef SUNDILE_ENTT_H
#define SUNDILE_ENTT_H

#include "../../globals/Common.h"
#include "EventTypes.h"

SYSTEM(EventSystem)
	inline bool run = true;
	inline std::vector<SmartEVW> EVWs;
	static SmartEVW currentEVW;

	SmartEVW create();

	SmartEVW create() {
		SmartEVW evw = std::make_shared<EventWrapper>();
		evw->id = EVWs.size();
		EVWs.push_back(evw);
		if (currentEVW.use_count() == 0) {
			currentEVW = evw;
		}
		return evw;
	}

	void init(SmartEVW evw) {
		evw->dispatcher.trigger<InitEvent>(evw);
		currentEVW = evw;
	}

	void update() {
		if (EVWs.size() == 0) {
			run = false;
			return;
		}

		for (SmartEVW evw : EVWs) {
			currentEVW = evw;
			evw->dispatcher.trigger<PreStepEvent>();
			evw->dispatcher.trigger<StepEvent>();
			evw->dispatcher.trigger<PostStepEvent>();
			evw->dispatcher.clear();
		}
	}

	void terminate(SmartEVW evw) {
		evw->dispatcher.update<TerminateEvent>();
		removeErase<SmartEVW>(EVWs, evw);
	}

END_SYSTEM

#include "EventUtilities.h"
#endif