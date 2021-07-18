#pragma once
#ifndef SUNDILE_ENTT_H
#define SUNDILE_ENTT_H

#include "../../globals/Common.h"
#include "EventTypes.h"

SYSTEM(EventSystem)
	inline bool run = true;
	inline std::vector<SmartEVW> EVWs;
	static SmartEVW currentEVW;

	void catchTerminate(TerminateEvent<SmartEVW>& tev) {
		run = false;
		EVWs.clear();
	}

	SmartEVW create() {
		SmartEVW evw = std::make_shared<EventWrapper>();
		evw->id = EVWs.size();
		EVWs.push_back(evw);
		if (currentEVW.use_count() == 0) {
			currentEVW = evw;
		}

		evw->dispatcher.sink<TerminateEvent<SmartEVW>>().connect<catchTerminate>();

		return evw;
	}

	void init(SmartEVW evw) {
		evw->dispatcher.trigger<InitEvent<SmartEVW>>({ evw });
		evw->dispatcher.trigger<ReadyEvent<SmartEVW>>({ evw });
		currentEVW = evw;
	}

	void update() {
		if (EVWs.size() == 0 || !run) {
			currentEVW->dispatcher.trigger<TerminateEvent<SmartEVW>>({ currentEVW });
			return;
		}

		for (SmartEVW evw : EVWs) {
			currentEVW = evw;

			evw->dispatcher.trigger<PreStepEvent<SmartEVW>>({evw});
			evw->dispatcher.trigger<StepEvent<SmartEVW>>({ evw });
			evw->dispatcher.trigger<PostStepEvent<SmartEVW>>({ evw });
			evw->dispatcher.clear();
		}
	}

	void terminate(SmartEVW evw) {
		evw->dispatcher.update<TerminateEvent<SmartEVW>>();
		removeErase<SmartEVW>(EVWs, evw);
	}


END_SYSTEM

#include "EventUtilities.h"
#endif