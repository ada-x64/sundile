#ifndef SUNDILE_EVENT_UTILITIES
#define SUNDILE_EVENT_UTILITIES
namespace sundile {
	//-- Utilities
	inline SmartEVW getEVWByID(unsigned int id) {
		for (auto evw : sundile::EventSystem::EVWs) {
			if (evw->id == id) return evw;
		}
		return nullptr;
	}

	inline entt::registry* getRegistryByID(SmartEVW evw, unsigned int id) {
		SimRegistryQuery srq;
		registryWrapper wrapper;
		srq.wrapper = &wrapper;
		srq.id = id;
		evw->dispatcher.trigger<SimRegistryQuery>(srq);
		return wrapper.registry;
	}
	inline Vec2 getWindowSizeByID(SmartEVW evw, unsigned int id) {
		WindowSizeQuery q;
		Vec2 vec;
		q.id = id;
		q.size = &vec;
		evw->dispatcher.trigger<WindowSizeQuery>(q);
		return vec;
	}
}
#endif