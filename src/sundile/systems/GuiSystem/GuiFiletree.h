#ifndef GUI_FILETREE
#define GUI_FILETREE
SYSTEM(GuiSystem)

namespace Filetree {

	struct filetreeNode {
		fs::path path;
		std::vector<filetreeNode> children;
	};

	static filetreeNode root;


	filetreeNode scanFiletree(fs::path rootPath) {
		filetreeNode root;
		root.path = rootPath;
		for (const auto& path : fs::directory_iterator(root.path)) {
			filetreeNode child;
			child.path = path;
			if (fs::is_directory(path)) {
				child = scanFiletree(path);
			}
			root.children.push_back(child);
		}

		return root;
	}

	void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
	{
		// Get the first occurrence
		size_t pos = data.find(toSearch);
		// Repeat till end is reached
		while (pos != std::string::npos)
		{
			// Replace this occurrence of Sub String
			data.replace(pos, toSearch.size(), replaceStr);
			// Get the next occurrence from the current position
			pos = data.find(toSearch, pos + replaceStr.size());
		}
	}

	void renderNode(filetreeNode node, int id) {
		for (auto& child : node.children) {
			std::stringstream ss;
			ss << child.path;
			std::string str = ss.str();
			findAndReplaceAll(str, "\\", "/");
			findAndReplaceAll(str, "\"", "");
			auto pos = str.rfind("/");
			str.replace(str.begin(), str.begin() + pos + 1, "");

			if (ImGui::TreeNode(str.c_str())) {
				renderNode(child,id);
				ImGui::TreePop();
			}
		}
	}

	void render(guiContainer& container) {
		renderNode(root, 0);
	}

	void init() {

		guiContainer fileDialog("file dialog", Filetree::render);
		primaryGuiContainer.state["file dialog"] = true;
		primaryGuiContainer.children.push_back(fileDialog);

		root = scanFiletree(ProjectSystem::currentProject->projectRoot);
	}
	void destroy() {

	}
}

END_SYSTEM
#endif