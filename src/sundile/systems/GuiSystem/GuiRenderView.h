#ifndef GUI_RENDER_VIEW
#define GUI_RENDER_VIEW

SYSTEM(GuiSystem)
namespace RenderView {

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoBackground;
	bool lockWindowRatio = false;
	float windowRatio[2] = { 16.f, 9.f };

	void render(guiContainer& container) {
		auto scene = SceneSystem::currentScene;
		Vec2 size = ImGui::GetWindowSize();
		Vec2 pos = ImGui::GetWindowPos();

		//Scene selector
		if (ImGui::Begin("RenderView Settings", NULL, ImGuiWindowFlags_NoResize || ImGuiWindowFlags_NoCollapse || ImGuiWindowFlags_NoMove )) {
			for (auto scene : SceneSystem::scenes) {
				ImGui::SameLine();
				if (ImGui::Button(scene->name.c_str())) {
					ActivateEvent<SmartScene> ev;
					ev.member = scene;
					EventSystem::currentEVW->dispatcher.trigger<ActivateEvent<SmartScene>>(ev);
				}
			}
			ImGui::SetWindowSize({ size.x, 100 });
			ImGui::SetWindowPos({ pos.x, pos.y + size.y });
			float newsize[2] = { size.x, size.y };

			if (lockWindowRatio) {
				newsize[1] = newsize[0] * windowRatio[1] / windowRatio[0];
			}

			ImGui::InputFloat2("Window Size", newsize);
			ImGui::SetWindowSize("render view", { newsize[0], newsize[1] });


			ImGui::Checkbox("Lock window ratio", &lockWindowRatio);
			ImGui::InputFloat2("Ratio", windowRatio, "%.1f");
		}
		ImGui::End();

		RenderSystem::setBbox(scene->renderer, { pos.x,pos.y,size.x,size.y });

		SceneSystem::currentScene->input.disabled = !ImGui::IsWindowHovered();
		if (ImGui::IsWindowHovered()) {
			container.windowFlags = windowFlags || ImGuiWindowFlags_NoMove;
		}
		else container.windowFlags = windowFlags;

		EventSystem::currentEVW->dispatcher.trigger<RenderEvent<SmartScene>>({ scene });
	}

	void destroy(TerminateEvent<SmartEVW>& ev) {

	}
	void init() {
		guiContainer renderView("render view", RenderView::render);
		renderView.windowFlags = windowFlags;
		primaryGuiContainer.state["render view"] = true;
		primaryGuiContainer.children.push_back(renderView);
		EventSystem::currentEVW->dispatcher.sink<TerminateEvent<SmartEVW>>().connect<&destroy>();
	}
}

END_SYSTEM

#endif