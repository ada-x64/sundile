//--
//-- GuiSystem.h
//--

#ifndef GUI_H
#define GUI_H
#ifndef SUNDILE_EXPORT
//--
//-- GuiSystem
//--
namespace sundile {
	// Contains typeinfo for registered components.
	struct guiMeta {
		void* ref;
		entt::id_type id = -1;
		entt::entity entt;
	};
	// Contains the Dear IMGUI instructions for registered components
	typedef std::function<void(guiMeta&)> guiRenderFunc;

	template <typename T>
	auto meta_cast(guiMeta meta) {
		return (T*)(meta.ref);
	}

	namespace Components {};

	namespace GuiSystem {
		using namespace Components;

		//-- Variables & Typedefs


		// Null members
		guiMeta nullMeta;
		void nullRenderFunc(guiMeta&) { ImGui::Text("(empty)"); }

		// Contains render functions
		struct guiIndex {
			std::string name = "(unregistered component(s))";
			guiRenderFunc f = nullRenderFunc; //by default, do nothing
			entt::id_type id = -1;
		};
		// Contains metas, which have the actual values to be passed to the function stored in the corresponding guiIndex
		struct listComponent {
			guiIndex index;
			guiMeta meta;
		};
		// Contains components
		struct listEntity {
			entt::entity entity = entt::null;
			std::string name = "(unset entity)";
			std::vector<listComponent> componentList = {};
		};
		// Contains primary GUI
		struct guiContainer {
			std::map<const GuiStateKey, bool> state;
			std::function<void()> renderFunc;
			guiContainer() : renderFunc([](){}) {};
			guiContainer(std::function<void()> renderFunc) : renderFunc(renderFunc) {};
		};

		// Vectors
		static std::vector<guiIndex> guiIndices;
		static std::vector<guiMeta> metaList;
		static std::vector<listEntity> entityList;


		//-- Functions
		auto checkContext() {
			auto ctx = ImGui::GetCurrentContext();
			if (!ctx) { ctx = ImGui::CreateContext(); ImGui::SetCurrentContext(ctx); }
			return ctx;
		}

		void terminate(const terminateEvent& ev) {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		//inspector front end, called every frame
		void EntityInspector(const SmartSim& sim) {
			auto ctx = checkContext();
			
			for (listEntity e : entityList) {
				if (ImGui::TreeNode(e.name.c_str())) {
					if (e.componentList.empty()) {
						ImGui::Text("(empty)");
						ImGui::TreePop();
						continue;
					}
					for (listComponent c : e.componentList) {
						if (ImGui::TreeNode(c.index.name.c_str())) {
							c.index.f(c.meta);
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
		}

		//back end - to be called according to a timer (every second?)
		void refreshEntities(SimStepEvent& sim) {

			//for every entity:
			//	get the entity's component IDs.
			//	if there is an associated listEntity then copy it.
			//	otherwise create a new one.
			//	for every component ID:
			//		if the ID matches a captured ID:
			//			find or create a listComponent

			// Only call once per second.
			float whole, fractional;
			fractional = std::modf(sim.currentTime, &whole);
			if (fractional > 2*sim.deltaTime) return;


			auto registry = sim.registry;
			registry->each([=](entt::entity e) {

				//Find or create guiEntity
				listEntity guiEntity;
				for (auto _e : entityList) {
					if (_e.entity == e) {
						guiEntity = _e;
						break;
					}
				}
				if (guiEntity.entity == entt::null) {
					std::string name = "Entity #" + std::to_string(int(e));
					listEntity _e;
					_e.entity = e;
					_e.name = ("Entity #" + std::to_string(int(e)));
					guiEntity = _e;
				}

				//Find or create component
				if (!registry->orphan(e)) {
					registry->visit(e, [&](const entt::id_type id) {
						for (auto& meta : metaList) {
							if (meta.id == id) {
								//Find or create listComponent
								listComponent component;
								for (listComponent& c : guiEntity.componentList) {
									if (id == c.index.id) {
										component = c;
										break;
									}
								}

								//Initialize empty listComponent
								if (component.meta.id == -1) {
									//set meta
									component.meta = meta;

									//get guiIndex
									for (guiIndex& i : guiIndices) {
										if (i.id == component.meta.id) {
											component.index = i;
											break;
										}
									}
								}

								//Replace or push_back
								bool set = false;
								for (auto& _c : guiEntity.componentList) {
									if (_c.index.id == component.index.id) {
										_c = component;
										set = true;
										break;
									}
								}
								if (!set) {
									guiEntity.componentList.push_back(component);
								}

								break;
							}
						}
					});
				}

				//Add guiEntity to entityList
				bool set = false;
				for (auto& _e : entityList) {
					if (_e.entity == e) {
						_e = guiEntity;
						set = true;
						break;
					}
				}
				if (!set) {
					entityList.push_back(guiEntity);
				}
			});
		}

		void render(const RenderGuiEvent& ev) {
			checkContext();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//Render all GUI elements here
			ev.registry->view<guiContainer>().each([&](auto& e, guiContainer& el) {
				el.renderFunc();
				});

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void registerECS(SmartWindow& winc, SmartSim& sim, SmartEVW& evw) {
			auto registry = sim->registry;
			float ww = winc->WIDTH;
			float wh = winc->HEIGHT;
			float viewport_w = 800;
			float viewport_h = 600;
			float viewport_x = ww / 2 - viewport_w / 2;
			float viewport_y = wh / 2 - viewport_h / 2;
			glViewport(viewport_x, viewport_y, viewport_w, viewport_h);

			auto GUI = registry->create();
			auto& e = registry->emplace<guiContainer>(GUI);
			e.renderFunc = [=, &e]() {
				//Variables
				using namespace ImGui;
				using k = GuiStateKey;
				ImVec2 windowSize = WindowSystem::getWindowSize(winc);
				auto& io = ImGui::GetIO();

				// Focus
				if (io.WantCaptureMouse) {
					GuiEvent event({ k::focusAny, true });
					evw->dispatcher.trigger<GuiEvent>(event);
				}

				// Main Menu Bar
				if (BeginMainMenuBar()) {
					if (BeginMenu("File")) {
						ImGui::EndMenu();
					}
					if (BeginMenu("Scene")) {
						ImGui::EndMenu();
					}
					if (BeginMenu("Window")) {
						std::string label;

						label = e.state[k::entityInspector] ? "Hide Entity Inspector" : "Show Entity Inspector";
						if (MenuItem(label.c_str()))
							e.state[k::entityInspector] = !e.state[k::entityInspector];

						label = e.state[k::componentInspector] ? "Hide Component Inspector" : "Show Component Inspector";
						if (MenuItem(label.c_str()))
							e.state[k::componentInspector] = !e.state[k::componentInspector];

						ImGui::EndMenu();
					}
					EndMainMenuBar();
				}

				//-- STATE ROUTER
				// ECS Tools
				if (e.state[k::entityInspector]) {
					Begin("ECS Tools");

					BeginChild("Entity Inspector");
					EntityInspector(sim);
					EndChild();

					End();
				}
			};
		}
		void simInit(const SimInitEvent& ev) {
			ev.evw->dispatcher.sink<SimStepEvent>().connect<refreshEntities>();
			ev.evw->dispatcher.sink<RenderGuiEvent>().connect<render>();
			ev.evw->dispatcher.sink<terminateEvent>().connect<terminate>();
		}
		void init(SmartWindow winc, SmartSim sim, SmartEVW evw) {
			//Initalize
			IMGUI_CHECKVERSION();
			checkContext();
			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(winc->window.get(), true);
			ImGui_ImplOpenGL3_Init(SUNDILE_GLSL_VERSION);

			//Setup font
			ImGuiIO& io = ImGui::GetIO();
			unsigned char* tex_pixels = NULL;
			int tex_w, tex_h;
			io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);

			registerECS(winc, sim, evw);
			evw->dispatcher.sink<SimInitEvent>().connect<GuiSystem::simInit>();
		}
	}

	//-- Global scope definition function
	template <typename T>
	void defineGui(guiRenderFunc f, std::string name = T::__name) {
		T foo;
		auto meta = entt::meta_any(foo);
		printf("GUI DEFINED FOR TYPE %s\nTYPE_ID:%i\n", typeid(T).name(), meta.type().type_id());
		GuiSystem::guiIndices.push_back(GuiSystem::guiIndex{ name, f, meta.type().type_id() });
	}
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
		guiMeta gm{&returned, meta.type().type_id(), entt};
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
	bool DragVec2(const char* name, Vec2& val, float v_speed = (1.0F), float v_min = (0.0F), float v_max = (0.0F), const char* format = "%.3f", float power = (1.0F)){
		float f[2] = { val.x, val.y };
		bool changed = ImGui::DragFloat2(name, f);
		val = f;
		return changed;
	}
	bool DragVec3(const char* name, Vec3& val, float v_speed = (1.0F), float v_min = (0.0F), float v_max = (0.0F), const char* format = "%.3f", float power = (1.0F)){
		float f[3] = { val.x, val.y, val.z };
		bool changed = ImGui::DragFloat3(name, f);
		val = f;
		return changed;
}
	bool DragVec4(const char* name, Vec4& val, float v_speed = (1.0F), float v_min = (0.0F), float v_max = (0.0F), const char* format = "%.3f", float power = (1.0F)){
		float f[4] = { val.x, val.y, val.z, val.w };
		bool changed = ImGui::DragFloat4(name, f);
		val = f;
		return changed;
	}




}

#else //ifdef SUNDILE_EXPORT

namespace GuiSystem {
	void init(SmartWindow winc, SmartSim sim, SmartEVW evw) {}
}

namespace sundile {
	template <typename T>
	void defineGui(std::function<void(std::any)> f, const char* name = T::__name) {}
}

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

#endif //end ifdef SUNDILE_EXPORT
#endif //end ifndef GUI_H