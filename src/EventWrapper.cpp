//--
//-- Entt.cpp
//--

#include "./headers/sundile/EventWrapper.h"

namespace sundile {
	//--
	//-- Events
	//--
	template <typename t>
	void EventWrapper::getEvent(Event e) {
		dispatcher.enqueue<t>(e);
	}

	void EventWrapper::step() {
		dispatcher.update();
	}

	void EventWrapper::terminate() {}

}