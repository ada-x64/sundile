//--
//-- GuiSystem.cpp
//--

#include "../components/GuiElement.h"

SYSTEM_DEF_BEGIN(GuiSystem)

ImGuiContext* ctx = ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();

void render(const GuiEvent& ev) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Render all GUI elements here
	//TODO - Create c_GuiElement and iterate through them here.
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
	ev.evw->dispatcher.sink<GuiEvent>().connect<render>();
	ev.evw->dispatcher.sink<terminateEvent>().connect<terminate>();
}

void init(GLFWwindow* window, const char* glsl_version) {
	//Initalize
	IMGUI_CHECKVERSION();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//Setup font
	unsigned char* tex_pixels = NULL;
	int tex_w, tex_h;
	io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);
}

SYSTEM_DEF_END