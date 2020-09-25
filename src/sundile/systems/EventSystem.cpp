//--
//-- Entt.cpp
//--

#include "EventSystem.h"
#include "../globals/Utility.h"

namespace sundile {
	namespace EventSystem {

		SmartEVW create() {
			SmartEVW evw = std::make_shared<EventWrapper>();
			EVWs.push_back(evw);
			return evw;
		}

		void init(SmartEVW evw) {
			evw->dispatcher.trigger<initEvent>();
		}

		void initAll() {
			for (SmartEVW evw : EVWs) {
				evw->dispatcher.trigger<initEvent>();
			}
		}

		void updateAll() {
			if (EVWs.size() == 0) {
				run = false;
				return;
			}
			
			for (SmartEVW evw : EVWs) {
				evw->dispatcher.trigger<preStepEvent>();
				evw->dispatcher.trigger<stepEvent>();
				evw->dispatcher.trigger<postStepEvent>();
			}
		}

		void terminate(SmartEVW evw) {
			evw->dispatcher.update<terminateEvent>();
			removeErase<SmartEVW>(EVWs, evw);
		}

		void terminateAll() {
			for (SmartEVW evw : EVWs) {
				evw->dispatcher.update<terminateEvent>();
				removeErase<SmartEVW>(EVWs, evw);
			}
		}
	}
}