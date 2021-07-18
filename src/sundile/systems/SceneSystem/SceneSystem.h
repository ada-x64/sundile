//--
//-- SceneSystem.h
//--
#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include "../EventSystem/EventSystem.h"

SYSTEM(SceneSystem)
	static std::vector<SmartScene> scenes = std::vector<SmartScene>();
	static SmartScene currentScene;

	//--
	//-- Creation
	//--
	template <typename T>
	T emplace(SmartScene scene, entt::entity entt) {
		auto returned = scene->registry->emplace<T>(entt);
		auto meta = entt::meta_any(returned);
		guiMeta gm{ &returned, meta.type().type_id(), entt };
		scene->guiMetas.push_back(gm);
		return returned;
	}

	template <typename T, typename ...Args>
	T emplace(SmartScene scene, entt::entity entt, Args &&...args) {
		auto returned = scene->registry->emplace<T>(entt, args...);
		auto meta = entt::meta_any(returned);
		guiMeta gm{ &returned, meta.type().type_id(), entt };
		scene->guiMetas.push_back(gm);
		return returned;
	}

	entt::entity addCoords(SmartScene scene) {
		/**/
		using namespace Systems;
		auto eCoords = scene->registry->create();
		Vertex v0{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vx{ glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vy{ glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vz{ glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) };
		Vertex vnx{ glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vny{ glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f) };
		Vertex vnz{ glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f) };
		Mesh coords = Mesh({ v0, vx, vy, vz, vnx, vny, vnz }, { 0,1,0, 0,2,0, 0,3,0, 0,4,0, 0,5,0, 0,6,0 }, {});
		emplace<Mesh>(scene, eCoords, coords);
		return eCoords;
		/**/
	}

	//--
	//-- ListEntities
	//--

	//TODO: Rewrite this to allow persistent entity names
	//THIS WILL BE THE GATEWAY TO SERIALIZATION!
	listEntity createListEntity(entt::entity e) {
		//Find or create guiEntity
		listEntity guiEntity;
		for (auto _e : SceneSystem::currentScene->entityList) {
			if (_e.entity == e) {
				guiEntity = _e;
				break;
			}
		}
		if (guiEntity.entity == entt::null) {
			std::string name = "Entity #" + std::to_string(int(e));
			guiEntity.entity = e;
			guiEntity.name = ("Entity #" + std::to_string(int(e)));
		}
		return guiEntity;
	}

	void updateEntities(SmartScene scene) {
		//for every entity:
		//	get the entity's component IDs.
		//	if there is an associated listEntity then copy it.
		//	otherwise create a new one.
		//	for every component ID:
		//		if the ID matches a captured ID:
		//			find or create a listComponent

		//scene->entityList.clear();
		auto& registry = scene->registry;
		registry->each([&](entt::entity e) {
			
			//Find or create listEntity
			listEntity guiEntity;
			for (auto& ge : scene->entityList) {
				if (ge.entity == e) {
					guiEntity = ge;
					break;
				}
			}
			if (guiEntity.entity == entt::null)
				guiEntity = createListEntity(e);

			//Find or create component
			if (!registry->orphan(e)) {
				registry->visit(e, [&](const entt::id_type id) {
					for (auto& meta : scene->guiMetas) {
						if (meta.id == id) {
							//Find or create listComponent
							listComponent component;
							for (listComponent& c : guiEntity.componentList) {
								if (id == c.index.id) {
									component = c;
									break;
								}
							}

							//Initialize empty listComponent
							if (component.meta.id == -1) {
								//set meta
								component.meta = meta;

								//get guiIndex
								for (guiIndex& i : guiIndices) {
									if (i.id == component.meta.id) {
										component.index = i;
										break;
									}
								}
							}

							//Replace or push_back
							bool set = false;
							for (auto& _c : guiEntity.componentList) {
								if (_c.index.id == component.index.id) {
									_c = component;
									set = true;
									break;
								}
							}
							if (!set) {
								guiEntity.componentList.push_back(component);
							}

							break;
						}
					}
					});
			}

			//Add guiEntity to entityList
			bool set = false;
			for (listEntity& _e : scene->entityList) {
				if (_e.entity == e) {
					_e = guiEntity;
					set = true;
					break;
				}
			}
			if (!set) {
				scene->entityList.push_back(guiEntity);
			}
			});
	}

	//--
	//-- Main Loop
	//--
	void update() {
		SmartScene& scene = currentScene;
		//-- Time
		scene->currentTime = glfwGetTime();
		scene->deltaTime = scene->currentTime - scene->lastTime;
		scene->lastTime = scene->currentTime;

		SmartEVW evw = EventSystem::currentEVW;

		//-- Update Events
		evw->dispatcher.update<InputEvent<SmartScene>>();
		updateEntities(scene);
		evw->dispatcher.trigger<PreStepEvent<SmartScene>>({ scene });
		evw->dispatcher.trigger<StepEvent<SmartScene>>({ scene });
		evw->dispatcher.trigger<PostStepEvent<SmartScene>>({ scene });
	}

	//Calls every time the scene is activated.
	void start(SmartScene scene) {
		// Required entities
		auto eCam = scene->registry->create();
		emplace<Camera>(scene, eCam);

		//addCoords(scene);

		//TODO: Unpack scene file
	}

	//Parses GLFW key events and passes appropriate information to other functions.
	void handleInput(const InputEvent<SmartWindow>& wev) {
		auto& input = wev.member->input;
		float cx = input.cursorpos.x * wev.member->WIDTH;
		float cy = input.cursorpos.y * wev.member->HEIGHT;

		Vec2 size = currentScene->renderer.size;
		Vec2 pos = currentScene->renderer.pos;

		float xx = cx - pos.x;
		float yy = cy - pos.y;
		Vec2 normalizedCoords = { xx / size.x, yy / size.y };
		InputSystem::interpretCursorPos(currentScene->input, normalizedCoords);

		for (int i = 0; i < (int)(btn::COUNT); ++i) {
			currentScene->input.pressed[i] = input.pressed[i];
			currentScene->input.held[i] = input.held[i];
			currentScene->input.released[i] = input.released[i];
		}

		if (InputSystem::isPressed(currentScene->input, btn::vk_escape)) {
			printf("Exiting on ESCAPE");
			exit(EXIT_FAILURE);
		}
	}

	SmartScene create(SmartEVW evw, std::string name = "NEW SCENE") {
		// Initialize
		SmartScene scene = std::make_shared<Scene>();
		scene->registry = std::make_shared<entt::registry>();
		scene->id = rand();
		scene->name = name;
		scene->renderer = RenderSystem::create();

		//Add to scenes
		scenes.push_back(scene);

		//Ensure scene registry isn't empty
		if (currentScene.use_count() == 0) {
			ActivateEvent<SmartScene> ev;
			ev.member = scene;
			evw->dispatcher.trigger(ev);
		}

		return scene;
	}

	void closeScene(SmartScene scene) {
		scene->close(scene);
		scene->registry->clear();
		scene->entityList.clear();
	}
	void openScene(SmartScene scene) {
		if (currentScene)
			closeScene(currentScene);

		start(scene);
		scene->open(scene);
		currentScene = scene;
		updateEntities(scene);
	}

	void activateScene(ActivateEvent<SmartScene>& ev) {
		openScene(ev.member);
		EventSystem::currentEVW->dispatcher.trigger<ReadyEvent<SmartScene>>(ev.member);

	}
	void deactivateScene(DeactivateEvent<SmartScene>& ev) {
		closeScene(ev.member);
	}

	//\TODO: Start implementing editor tools
	void parseGuiState(std::map<std::string, bool>& map) {

	}

	void init(SmartEVW evw) {
		// Connect event listeners
		evw->dispatcher.sink<StepEvent<SmartEVW>>().connect<update>();
		evw->dispatcher.sink<InputEvent<SmartWindow>>().connect<handleInput>();

		evw->dispatcher.sink<ActivateEvent<SmartScene>>().connect<activateScene>();
		evw->dispatcher.sink<DeactivateEvent<SmartScene>>().connect<deactivateScene>();
		//Default scene
		create(evw);
	}

	// SERIALIZATION

	template<typename Archive, typename SnapshotHandler>
	void handleArchive(SmartScene scene) {
		std::fstream fstream;
		fstream.open(ProjectSystem::currentProject->projectRoot.string() + scene->name + ".sunScene", std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);

		{
			Archive archive{ fstream };
			archive(SUNDILE_VERSION_NUMBER);
			archive(*(ProjectSystem::currentProject.get()));
			SnapshotHandler{ *(scene->registry.get()) }.entities(archive).component<guiMeta>();
		}

		fstream.close();
	}

	void saveScene() {
		ProjectSystem::currentProject->evw->dispatcher.trigger<SerializeEvent<SmartScene>>();
		handleArchive<cereal::BinaryOutputArchive, entt::snapshot>(SceneSystem::currentScene);
	}

	void loadScene(fs::path fullPath) {
		//Clear out the current project
		SmartScene loadedScene = std::make_shared<Scene>();
		ProjectSystem::currentProject->evw->dispatcher.trigger<DeserializeEvent<SmartProject>>();
		handleArchive<cereal::BinaryInputArchive, entt::snapshot_loader>(loadedScene);
		openScene(loadedScene);
	}

END_SYSTEM

template<typename Archive>
void serialize(Archive& ar, sundile::Components::Scene& scene) {
	ar(scene.name, scene.registry);
}
#endif
