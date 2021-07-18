#ifndef GUI_NODE_VIEW
#define GUI_NODE_VIEW

SYSTEM(GuiSystem)

namespace NodeView {

    namespace node = ax::NodeEditor;
	struct Pin {
		node::PinId id;
		node::PinKind kind;
		bool visible;
		std::string name = "New Pin";
		Pin(node::PinId id, std::string name, node::PinKind kind) : id(id), name(name), kind(kind) {};
		bool operator ==(const Pin& other) const { return this->id.Get() == other.id.Get(); }
	};
	using SmartPin = std::shared_ptr<Pin>;
	struct Node;
	using SmartNode = std::shared_ptr<Node>;
	enum class NodeType {
		Parent,
		Scene,
		Entity,
		Component
	};
	struct Node {
		NodeType type;
		node::NodeId id = -1;
		std::vector<SmartPin> pins;
		std::vector<SmartNode> children;
		bool visible = true;
		std::string name = "New Node";
		char nameBuffer[255];
		bool operator< (const Node& other) const { return this->id.Get() < other.id.Get(); }
		Node(node::NodeId id) : id(id) {
			strcpy(nameBuffer, name.c_str());
		};
		Node() {
			strcpy(nameBuffer, name.c_str());
		};
	};
	struct Link
	{
		node::LinkId id = 0;
		SmartPin input;
		SmartPin output;
		SmartNode to;
		SmartNode from;
		Link(node::LinkId id, SmartPin input, SmartPin output, SmartNode from, SmartNode to)
			: id(id), input(input), output(output), to(to), from(from) {};
		Link() = default;
	};
	static Link nullLink;


	template <typename DataType>
	struct NodeWrapper : std::map<SmartNode, std::shared_ptr<DataType>> {};

	static node::EditorContext* nodeCtx = nullptr;
    static ImVector<Link> links;
	NodeWrapper<Scene> sceneWrapper; // Singlet, points to all scenes, cannot be deleted
	SmartNode parentNode = std::make_shared<Node>(0);
	NodeWrapper<listEntity> entityWrapper; //size = |scenes|
	NodeWrapper<listComponent> componentWrapper; //size = |entites|


	/*
	|ParentNode  |         .->|SceneNode |             .->|EntityNode   |                .->|ComponentNode|
	|------------|         |  |----------|             |  |-------------|                |  |-------------|
	|Scene A    >|---------|  |Entity A >|-------------|  |Component A >|----------------|  |meta.render()|
                           |                           |                                 |                  
	SceneWrapper[SmartScene]-->EntityWrapper[listEntity]-->ComponentWrapper[listComponent]-->dataWrapper[guiMeta]
	*/

	Link& getLink(SmartNode to, SmartNode from) {
		for (auto& link : links) {
			if (link.to == to && link.from == from) {
				return link;
			}
		}
		return nullLink;
	}
	Link& getLink(SmartPin output) {
		for (auto& link : links) {
			if (link.output == output) {
				return link;
			}
		}
		return nullLink;
	}

	SmartNode getChildNode(SmartNode parent, node::NodeId id) {
		if (parent->id == id) return parent;
		for (auto child : parent->children) {
			if (child->id == id) return child;
			auto found = getChildNode(child, id);
			if (found) return found;
		}
		return nullptr;
	}
	SmartNode getNode(node::NodeId id) {
		//tree recursion
		return getChildNode(parentNode, id);
	}

	void clearNodes(SmartNode node) {
		for (auto& child : node->children) {
			clearNodes(child);
		}
		node->pins.clear();
		node->children.clear();
	}

	void populateNodes(Event<SmartScene> ev) {
		//-- Recursively clear all existing nodes
		clearNodes(parentNode);
		sceneWrapper.clear();
		entityWrapper.clear();
		componentWrapper.clear();
		links.clear();

		SceneSystem::updateEntities(ev.member);
		
		//-- Populate new nodes
		node::SetCurrentEditor(nodeCtx);
		int id = 2; //id 1 reserved for parent node

		auto& scene = SceneSystem::currentScene;
		SmartNode sceneNode = std::make_shared<Node>(id++);
		sceneWrapper[sceneNode] = scene;
		sceneNode->name = scene->name;
		sceneNode->type = NodeType::Scene;
		parentNode->children.push_back(sceneNode);

		SmartPin input = std::make_shared<Pin>(id++, ">", node::PinKind::Input);
		SmartPin output = std::make_shared<Pin>(id++, sceneNode->name, node::PinKind::Output);
		sceneNode->pins.push_back(input);
		parentNode->pins.push_back(output);
		links.push_back(Link(id++, input, output, parentNode, sceneNode));

		for (auto& entity : scene->entityList) {
			SmartNode entityNode = std::make_shared<Node>(id++);
			entityWrapper[entityNode] = std::make_shared<listEntity>(entity);
			entityNode->name = entity.name;
			entityNode->type = NodeType::Entity;
			sceneNode->children.push_back(entityNode);

			SmartPin input = std::make_shared<Pin>(id++, ">", node::PinKind::Input);
			SmartPin output = std::make_shared<Pin>(id++, entityNode->name, node::PinKind::Output);
			entityNode->pins.push_back(input);
			sceneNode->pins.push_back(output);
			links.push_back(Link(id++,input, output, sceneNode, entityNode));


			if (entity.componentList.size() == 0) continue;
			for (auto& component : entity.componentList) {
				SmartNode componentNode = std::make_shared<Node>(id++);
				componentWrapper[componentNode] = std::make_shared<listComponent>(component);
				componentNode->name = component.name;
				componentNode->type = NodeType::Component;
				entityNode->children.push_back(componentNode);

				SmartPin input = std::make_shared<Pin>(id++, ">", node::PinKind::Input);
				SmartPin output = std::make_shared<Pin>(id++, componentNode->name, node::PinKind::Output);
				componentNode->pins.push_back(input);
				entityNode->pins.push_back(output);
				links.push_back(Link(id++, input, output, entityNode, componentNode));

			}
		}

	}

	//Recursively renders wrappers, starting with SceneWrapper
	ImVec2 renderNode(SmartNode node, ImVec2 sumSize = { 0.f, 0.f }, ImVec2 prevSize = { 0.f, 0.f }) {

		//render
		node::BeginNode(node->id);
		ImGui::Text(node->name.c_str());
		std::string dashes = "";
		while (dashes.length() < node->name.length()) {
			dashes += "-";
		}
		ImGui::Text(dashes.c_str());
		ImGui::Text("%i", node->id.Get());

		//Pins
		for (auto& pin : node->pins) {
			node::BeginPin(pin->id, pin->kind);
			ImGui::Text(pin->name.c_str());
			/**/
			if (ImGui::IsItemClicked(0)) {
				auto& link = getLink(pin);
				if (link.id.Get() > 0)
					link.to->visible = !link.to->visible;
			}
			/**/
			node::EndPin();
		}

		//Render component node stuff here
		if (node->type == NodeType::Component && componentWrapper[node]) {
			auto& component = componentWrapper[node];
			component->index.renderFunc(component->meta);
		}

		//render done!
		node::EndNode();

		//set position
		const float padding = 10.f;
		const ImVec2 size = node::GetNodeSize(node->id.Get());

		float height = size.y + padding;
		float ypos = sumSize.y;

		float width = size.x + padding;
		float xpos = sumSize.x;

		sumSize.x += width;

		//recurse
		for (auto& child : node->children) {
			if (child->visible)
				sumSize = renderNode(child, sumSize);
		}

		node::SetNodePosition(node->id.Get(), { xpos, ypos });
		sumSize.y += height;
		sumSize.x -= width;
		//return size
		return sumSize;
	}

	static node::NodeId contextNodeId = 0;
	static node::LinkId contextLinkId = 0;
	static node::PinId  contextPinId = 0;

	static bool b_rename = false;

	void renderContextMenu() {
		node::Suspend();

		if (node::ShowNodeContextMenu(&contextNodeId)) {
			ImGui::OpenPopup("Node Context Menu");
		}
		if (node::ShowPinContextMenu(&contextPinId)) {
			ImGui::OpenPopup("Pin Context Menu");
		}
		if (node::ShowLinkContextMenu(&contextLinkId)) {
			ImGui::OpenPopup("Link Context Menu");
		}
		if (node::ShowBackgroundContextMenu()) {
			ImGui::OpenPopup("Background Context Menu");
		}
		if (b_rename) {
			ImGui::OpenPopup("Node Rename");
		}


		if (ImGui::BeginPopup("Node Context Menu")) {

			auto node = getNode(contextNodeId);

			if (node) {
				if (ImGui::MenuItem("Rename")) {
					strcpy(node->nameBuffer, node->name.c_str());
					b_rename = true;
				}
			}
			else {
				ImGui::Text("(error, you shouldn't see this)");
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("Pin Context Menu")) {

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("Link Context Menu")) {

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("Background Context Menu")) {

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Node Rename", &b_rename)) {
			auto node = getNode(contextNodeId);

			if (node) {
				if (ImGui::InputText("", node->nameBuffer, 255, ImGuiInputTextFlags_EnterReturnsTrue)
					|| ImGui::Button("Rename")) {
					node->name = node->nameBuffer;
					b_rename = false;

					switch (node->type) {
					case (NodeType::Scene):
						sceneWrapper[node]->name = node->name;
						break;
					case (NodeType::Entity):
						entityWrapper[node]->name = node->name;
						break;
					case (NodeType::Component):
						componentWrapper[node]->name = node->name;
						break;
					}
				}
			}
			else {
				ImGui::Text("(error, you shouldn't see this)");
			}
			ImGui::EndPopup();
		}
		node::Resume();
	}

	void render(guiContainer& container) {
		node::Begin("Node View");

		renderNode(parentNode);

        // Submit Links
        for (auto& linkInfo : links)
            node::Link(linkInfo.id, linkInfo.input->id, linkInfo.output->id);

		renderContextMenu();

		node::End();
	}

	void destroy(TerminateEvent<SmartEVW>& ev) {
		node::DestroyEditor(nodeCtx);
	}

	void init() {
		nodeCtx = node::CreateEditor();
		node::SetCurrentEditor(nodeCtx);

		guiContainer nodeView("node view", NodeView::render);
		primaryGuiContainer.state["node view"] = true;
		primaryGuiContainer.children.push_back(nodeView);
		EventSystem::currentEVW->dispatcher.sink<TerminateEvent<SmartEVW>>().connect<&destroy>();
		EventSystem::currentEVW->dispatcher.sink<ReadyEvent<SmartScene>>().connect<&populateNodes>();

		parentNode->name = "Scenes";
		parentNode->id = 1;
	}

}


END_SYSTEM

#endif