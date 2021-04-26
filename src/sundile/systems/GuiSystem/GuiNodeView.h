#ifndef GUI_NODE_VIEW
#define GUI_NODE_VIEW

SYSTEM(GuiSystem)

namespace NodeView {

	namespace node = ax::NodeEditor;
	static node::EditorContext* nodeCtx = nullptr;

	void render(guiContainer& container) {
		node::SetCurrentEditor(nodeCtx);
		node::Begin("Node View");

		int id = 0;
		node::BeginNode(id++);
		ImGui::Text("Node A");
		node::BeginPin(id++, node::PinKind::Input);
		ImGui::Text("-> In");
		node::EndPin();
		ImGui::SameLine();
		node::BeginPin(id++, node::PinKind::Output);
		ImGui::Text("Out ->");
		node::EndPin();
		node::EndNode();

		node::End();
	}

	void init() {
		nodeCtx = node::CreateEditor();

		guiContainer nodeView("node view", NodeView::render);
		primaryGuiContainer.state["node view"] = true;
		primaryGuiContainer.children.push_back(nodeView);

	}

	void destroy() {
		node::DestroyEditor(nodeCtx);
	}

}


END_SYSTEM

#endif