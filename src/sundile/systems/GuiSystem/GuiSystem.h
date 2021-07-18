#ifndef GUI_SYSTEM
#define GUI_SYSTEM

#include "GuiMainWrapper.h"
#include "GuiRenderView.h"
#include "GuiNodeView.h"
#include "GuiFiletree.h"
#include "GuiDebugView.h"

SYSTEM(GuiSystem)
void sendRenderEvent() {
	RenderEvent<guiContainer> ev;
	ev.member = primaryGuiContainer;
	EventSystem::currentEVW->dispatcher.trigger<RenderEvent<guiContainer>>(ev);
}
void render(RenderEvent<guiContainer>& ev) {
	if (EventSystem::run) {
		checkContext();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Render all GUI elements here
		//primaryGuiContainer.renderFunc(primaryGuiContainer);
		stateRouter(primaryGuiContainer);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
void windowInit(const InitEvent<SmartWindow>& ev) {
	auto win = guiRegistry.create();
	guiRegistry.emplace<windowID>(win, ev.member->id);

	ImGui_ImplGlfw_InitForOpenGL(ev.member->window, true);
	ImGui_ImplOpenGL3_Init(SUNDILE_GLSL_VERSION);

	if (currentWindow == -1) {
		currentWindow = ev.member->id;

		float ww = ev.member->WIDTH;
		float wh = ev.member->HEIGHT;
		float viewport_w = 800;
		float viewport_h = 600;
		float viewport_x = ww / 2 - viewport_w / 2;
		float viewport_y = wh / 2 - viewport_h / 2;
		glViewport(viewport_x, viewport_y, viewport_w, viewport_h);
	}
}

void init(SmartEVW evw) {
	//Initalize
	IMGUI_CHECKVERSION();
	checkContext();
	ImGui::StyleColorsDark();

	//Setup font
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* tex_pixels = NULL;
	int tex_w, tex_h;
	io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);

	//do event stuff
	evw->dispatcher.sink<InitEvent<SmartWindow>>().connect<windowInit>();
	evw->dispatcher.sink<RenderEvent<guiContainer>>().connect<&render>();
	evw->dispatcher.sink<TerminateEvent<SmartEVW>>().connect<&terminate>();
	evw->dispatcher.sink<PreStepEvent<SmartEVW>>().connect<&sendRenderEvent>();

	//Initialize front-end
	guiContainer main("primary container",
		[](guiContainer& container) -> void {
			auto evw = EventSystem::currentEVW;
			ImGui::SetWindowSize(primaryGuiContainer.name.c_str(), ImVec2(WindowSystem::currentWindow->WIDTH, WindowSystem::currentWindow->HEIGHT));
			ImGui::SetWindowPos(primaryGuiContainer.name.c_str(), ImVec2(0,0));
			container.state["focus any"] = ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);
			mainMenu(container);
		});

	primaryGuiContainer = main;
	primaryGuiContainer.windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;

	NodeView::init();
	RenderView::init();
	Filetree::init();
	DebugView::init();

}

END_SYSTEM

#endif