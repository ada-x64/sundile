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
		ImGuiContext* ctx = ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		entt::entity current;

		typedef std::pair<const char*, std::function<void(std::any)>> guiIndex;
		static std::vector<guiIndex> guiIndices;

		typedef std::function<void()> guiRenderFunc;

		void refreshECS(SmartSim sim) {
			auto& registry = sim->registry;
			registry->each([=](const entt::entity e) {
				});
		}

		void renderInspector(SmartSim sim) {
			//do stuff
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

		void render(const RenderGuiEvent& ev) {
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

		void terminate(const terminateEvent& ev) {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void simInit(const SimInitEvent& ev) {
			ev.evw->dispatcher.sink<RenderGuiEvent>().connect<render>();
			ev.evw->dispatcher.sink<terminateEvent>().connect<terminate>();
		}

		void init(SmartWindow winc, SmartSim sim, SmartEVW evw) {
			//Initalize
			IMGUI_CHECKVERSION();
			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(winc->window.get(), true);
			ImGui_ImplOpenGL3_Init(SUNDILE_GLSL_VERSION);

			//Setup font
			unsigned char* tex_pixels = NULL;
			int tex_w, tex_h;
			io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);

			registerECS(winc, sim);
			evw->dispatcher.sink<SimInitEvent>().connect<GuiSystem::simInit>();
		}
	}

	//-- Global scope definition function
	template <typename T>
	void defineGui(std::function<void(std::any)> f, const char* name = T::__name) {
		GuiSystem::guiIndices.push_back(GuiSystem::guiIndex( name, f ));
	}
}

#else //ifdef SUNDILE_EXPORT

BEGIN_COMPONENT(guiElement) END_COMPONENT
namespace GuiSystem {
	void init(winc, sim) {}
}


namespace sundile {
	template <typename T>
	void defineGui(std::function<void(std::any)> f, const char* name = T::__name) {}
}

#endif //end ifdef SUNDILE_EXPORT
#endif //end ifndef GUI_H