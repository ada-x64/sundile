//--
//-- GuiSystem.h
//--

#ifndef GUI_H
#define GUI_H
#ifndef SUNDILE_EXPORT
//--
//-- guiElement
//--
BEGIN_COMPONENT(guiElement)
	std::function<void()> renderFunc;
	guiElement(std::function<void()> renderFunc) : renderFunc(renderFunc) {};
END_COMPONENT

//--
//-- GuiSystem
//--
namespace sundile {
	namespace GuiSystem {
		using namespace Components;

		//-- Variables & Typedefs
		typedef std::function<void(entt::meta_any)> guiRenderFunc;
		struct guiIndex { const char* name; guiRenderFunc f; entt::id_type id; };
		std::vector<guiIndex> guiIndices;
		std::vector<entt::meta_any> metas;



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

		//TODO:
		//Display all entities in a tree. (done)
		//Show selected entities components in another tree. (done)
		//Lookup guiRenderFunc using component's name and call it. (wip)
		void renderInspector(SmartSim sim) {
			checkContext();
			auto registry = sim->registry;
			//TODO:
			//Refresh entity list on fousing the window
			//Refresh component list on 
			//This can be done by setting a listener to every entity in the primary registry ... is that a good idea?

			registry->each([=](entt::entity e) {
				std::string entt_name = "Entity #" + std::to_string(int(e)); //TODO: Allow rename
				if (ImGui::TreeNode(entt_name.c_str())) {
					if (registry->orphan(e)) return;
					registry->visit(e, [=](const entt::id_type id) { //For some reason this iteration is making a lot of duplicate values
						for (guiIndex i : guiIndices) {
							for (auto meta : metas) {
								auto meta_id = meta.type().type_id();
								if (meta_id == id && ImGui::TreeNode(i.name)) {
									i.f(meta);
									ImGui::TreePop();
								}
							}
						}
					});
					ImGui::TreePop();
				}
			});
		}
		void render(const RenderGuiEvent& ev) {
			checkContext();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//Render all GUI elements here
			ev.registry->view<guiElement>().each([&](auto& e, guiElement& el) {
				el.renderFunc();
				});

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void registerECS(SmartWindow winc, SmartSim sim) {
			auto registry = sim->registry;
			float ww = winc->WIDTH;
			float wh = winc->HEIGHT;
			float viewport_w = 800;
			float viewport_h = 600;
			float viewport_x = ww / 2 - viewport_w / 2;
			float viewport_y = wh / 2 - viewport_h / 2;
			glViewport(viewport_x, viewport_y, viewport_w, viewport_h);

			auto renderWindow = registry->create();
			registry->emplace<guiElement>(renderWindow, [=]() {
				using namespace ImGui;
				SetNextWindowBgAlpha(0.f);
				SetNextWindowSizeConstraints({ viewport_w, viewport_h }, { viewport_w, viewport_h });
				Begin("rendering frame");
				auto pos = ImGui::GetWindowPos();
				SetWindowSize({ 800, 600 });
				glViewport(pos.x, -pos.y, viewport_w, viewport_h); //magic number here - don't know what the vertical offset between imgui and gl is. i guess it's 101.
				End();

				GuiEvent e; //TODO: make this better
				e.content = GuiEventContent{ ImGui::IsWindowFocused() };
				sim->evw->dispatcher.trigger<GuiEvent>(e);
				});

			auto inspector = registry->create();
			ImVec2 inspectorSize = { 240, static_cast<float>(winc->WIDTH) };
			registry->emplace<guiElement>(inspector, [=]() {

				ImVec2 windowSize = WindowSystem::getWindowSize(winc);

				using namespace ImGui;
				SetNextWindowSize(inspectorSize);
				SetNextWindowSizeConstraints({ inspectorSize.x, 120 }, { inspectorSize.x, inspectorSize.y });
				//SetNextWindowPos({ windowSize.x - inspectorSize.x, 0 });

				Begin("Inspector");
				renderInspector(sim);
				End();
				});

			auto toolbar = registry->create();
			ImVec2 toolbarSize = { 120, static_cast<float>(winc->WIDTH) };
			registry->emplace<guiElement>(toolbar, [=]() {

				ImVec2 windowSize = WindowSystem::getWindowSize(winc);

				using namespace ImGui;
				SetNextWindowSize(toolbarSize);
				SetNextWindowSizeConstraints({ toolbarSize.x, 32 }, { windowSize.y, toolbarSize.y });
				SetNextWindowPos({ windowSize.x - toolbarSize.x - inspectorSize.x, 0 }); //this should lock the toolbar to the right. it isn't doing that x(
				SetWindowPos("Toolbar", { windowSize.x - toolbarSize.x - inspectorSize.x, 0 });

				Begin("Toolbar");
				Text("Entity tools:");
				Button("Add");
				Button("Select");
				Button("Translate");
				Button("Rotate");
				End();
				});
		}
		void simInit(const SimInitEvent& ev) {
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

			registerECS(winc, sim);
			evw->dispatcher.sink<SimInitEvent>().connect<GuiSystem::simInit>();
		}
	}

	//-- Global scope definition function
	template <typename T>
	void defineGui(GuiSystem::guiRenderFunc f, const char* name = T::__name) {
		GuiSystem::guiIndices.push_back(GuiSystem::guiIndex{ name, f });
	}

	template <typename T>
	T emplace(SmartRegistry registry, entt::entity entt) {
		auto returned = registry->emplace<T>(entt);
		auto meta = entt::meta_any(returned);
		GuiSystem::metas.push_back(meta);
		//TODO: Sink a function to remove meta from metas if necessary
		return returned;
	}

	template <typename T, typename ...Args>
	T emplace(SmartRegistry registry, entt::entity entt, Args ...args) {
		auto returned = registry->emplace<T>(entt, args...);
		auto meta = entt::meta_any(returned);
		GuiSystem::metas.push_back(meta);
		return returned;
	}

}

#else //ifdef SUNDILE_EXPORT

BEGIN_COMPONENT(guiElement) END_COMPONENT
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

#endif //end ifdef SUNDILE_EXPORT
#endif //end ifndef GUI_H