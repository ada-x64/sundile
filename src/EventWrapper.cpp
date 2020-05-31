//--
//-- Entt.cpp
//--

#include "./headers/sundile/EventWrapper.h"

namespace sundile {
	//--
	//-- Events
	//--
	void EventWrapper::getEvent(Event e) {
		dispatcher.enqueue(e);
	}

	void EventWrapper::step() {
		dispatcher.update();
	}

	void EventWrapper::terminate() {}

}