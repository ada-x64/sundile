#ifndef GUI_META_H
#define GUI_META_H
#include "GuiTypes.h"
#ifndef SUNDILE_EXPORT
namespace sundile {
	//--
	//-- META FUNCTIONS
	//--
	template <typename T>
	T* meta_cast(guiMeta meta) {
		return (T*)(meta.ref);
	}

	//-- Global scope definition function
	template <typename T>
	void defineGui(guiRenderFunc f, std::string name = T::__name) {
		T foo;
		auto meta = entt::meta_any(foo);
		printf("GUI DEFINED FOR TYPE %s\nTYPE_ID:%i\n", typeid(T).name(), meta.type().type_id());
		GuiSystem::guiIndices.push_back(GuiSystem::guiIndex{ name, f, meta.type().type_id() });
	}

	//TODO: REPLACE THIS FUNCTION
	template <typename T>
	void updateGUI(entt::entity entt, T& value) {
		using namespace GuiSystem;
		for (auto& e : entityList) {
			if (e.entity == entt) {
				auto meta_any = entt::meta_any(value);
				for (auto& c : e.componentList) {
					if (c.meta.id == meta_any.type().type_id()) {
						void* v = &value;
						c.meta.ref = v;
						return;
					}
				}
			}
		}
	}

	template <typename T>
	T emplace(SmartRegistry registry, entt::entity entt) {
		auto returned = registry->emplace<T>(entt);
		auto meta = entt::meta_any(returned);
		guiMeta gm{ &returned, meta.type().type_id(), entt };
		GuiSystem::metaList.push_back(gm);
		return returned;
	}

	template <typename T, typename ...Args>
	T emplace(SmartRegistry registry, entt::entity entt, Args &&...args) {
		auto returned = registry->emplace<T>(entt, args...);
		auto meta = entt::meta_any(returned);
		guiMeta gm{ &returned, meta.type().type_id(), entt };
		GuiSystem::metaList.push_back(gm);
		return returned;
	}

	//--
	//-- defineGui Helpers
	//--
	bool DragVec2(const char* name, Vec2& val, float v_speed = (1.0F), float v_min = (0.0F), float v_max = (0.0F), const char* format = "%.3f", float power = (1.0F)) {
		float f[2] = { val.x, val.y };
		bool changed = ImGui::DragFloat2(name, f);
		val = f;
		return changed;
	}
	bool DragVec3(const char* name, Vec3& val, float v_speed = (1.0F), float v_min = (0.0F), float v_max = (0.0F), const char* format = "%.3f", float power = (1.0F)) {
		float f[3] = { val.x, val.y, val.z };
		bool changed = ImGui::DragFloat3(name, f);
		val = f;
		return changed;
	}
	bool DragVec4(const char* name, Vec4& val, float v_speed = (1.0F), float v_min = (0.0F), float v_max = (0.0F), const char* format = "%.3f", float power = (1.0F)) {
		float f[4] = { val.x, val.y, val.z, val.w };
		bool changed = ImGui::DragFloat4(name, f);
		val = f;
		return changed;
	}
#else //ifdef SUNDILE_EXPORT
	namespace GuiSystem {
		void init(SmartEVW evw) {}
	}

	template <typename T>
	void defineGui(std::function<void(std::any)> f, const char* name = T::__name) {}

	template <typename T>
	T emplace(SmartRegistry registry, entt::entity entt, T component) {
		return registry->emplace<T>(component);
	}
	template <typename T, typename ...Args>
	T emplace(SmartRegistry registry, entt::entity entt, T component, Args ...args) {
		return registry->emplace<T>(component, args);
	}

	template <typename T>
	updateGUI(smartRegistry registry, entt::entity entt) {}
#endif //end ifndef SUNDILE_EXPORT
}
#endif