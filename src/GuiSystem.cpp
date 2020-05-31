//--
//-- GuiSystem.cpp
//--

#include "./headers/sundile/GuiSystem.h"

namespace sundile {
	namespace GuiSystem {
		ImGuiContext *ctx = ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		void Init(GLFWwindow *window, const char* glsl_version) {
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

		void Render() {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();


			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void Terminate() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	}
}