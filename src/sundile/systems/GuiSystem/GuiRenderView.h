#ifndef GUI_RENDER_VIEW
#define GUI_RENDER_VIEW

SYSTEM(GuiSystem)
namespace RenderView {

	void render(guiContainer& container) {
		auto scene = SceneSystem::currentScene;
		Vec2 size = ImGui::GetWindowSize();
		Vec2 pos = ImGui::GetWindowPos();
		RenderSystem::setSizePos(scene->renderer, { pos.x,pos.y,size.x,size.y });

		RenderEvent<SmartScene> rev;
		rev.member = scene;
		EventSystem::currentEVW->dispatcher.trigger<RenderEvent<SmartScene>>(rev);
	}

	void init() {
		guiContainer renderView("render view", RenderView::render);
		primaryGuiContainer.state["render view"] = true;
		primaryGuiContainer.children.push_back(renderView);
	}

	void destory() {

	}
}

END_SYSTEM

#endif