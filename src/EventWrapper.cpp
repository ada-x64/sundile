//--
//-- Entt.cpp
//--

#include "./headers/sundile/EventWrapper.h"

namespace sundile {
	namespace EventSystem {
		SmartEVW create() {
			SmartEVW evw = std::make_shared<EventWrapper>();
			EVWs.push_back(evw);
			return evw;
		}

		void init(SmartEVW evw) {
			evw->dispatcher.update<initEvent>();
		}

		void initAll() {
			for (SmartEVW evw : EVWs) {
				evw->dispatcher.update<initEvent>();
			}
		}

		void update(SmartEVW evw) {
			evw->dispatcher.update<preStepEvent>();
			evw->dispatcher.update<stepEvent>();
			evw->dispatcher.update<postStepEvent>();

			evw->dispatcher.update<preRenderEvent>();
			evw->dispatcher.update<renderEvent>();
			evw->dispatcher.update<postRenderEvent>();

			evw->dispatcher.update<preDrawGuiEvent>();
			evw->dispatcher.update<drawGuiEvent>();
			evw->dispatcher.update<postDrawGuiEvent>();
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
								
				evw->dispatcher.trigger<preRenderEvent>();
				evw->dispatcher.trigger<renderEvent>();
				evw->dispatcher.trigger<postRenderEvent>();
								
				evw->dispatcher.trigger<preDrawGuiEvent>();
				evw->dispatcher.trigger<drawGuiEvent>();
				evw->dispatcher.trigger<postDrawGuiEvent>();
			}
		}

		void terminate(SmartEVW evw) {
			evw->dispatcher.update<terminateEvent>();
			Utility::removeErase<SmartEVW>(EVWs, evw);
		}

		void terminateAll() {
			for (SmartEVW evw : EVWs) {
				evw->dispatcher.update<terminateEvent>();
				Utility::removeErase<SmartEVW>(EVWs, evw);
			}
		}
	}
}