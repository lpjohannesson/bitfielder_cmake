#pragma once
#include <vector>
#include "engine/scene.h"
#include "world/world.h"
#include "core/packet.h"
#include "client/server_connection.h"
#include "client/world/world_renderer.h"
#include "client/world/client_content.h"
#include "client/world/camera.h"
#include "client/menu/option_list.h"
#include "client/menu/text_list_option.h"
#include "client/world/entity/systems/client_entity_system.h"

namespace bf {
	class WorldScene : public Scene {
	public:
		ServerConnection *server;

		World world;
		WorldRenderer worldRenderer;
		Camera camera;

		std::vector<ClientEntitySystem*> entitySystems;

		OptionList pauseOptionList;
		TextListOption pauseContinueOption, pauseTitleOption;
		OptionListRenderer optionListRenderer;

		SpriteMesh pauseLogoMesh;

		bool paused = false;

		ClientContent clientContent;

		void updateBlock(glm::ivec2 position);
		void spawnBlockParticles(glm::vec2 position, entt::entity block);

		void placeBlock(int blockIndex, glm::ivec2 position, bool onFrontLayer, BlockData &blockData);
		void destroyBlock(glm::ivec2 position, bool onFrontLayer, BlockData &blockData);

		bool updatePauseMenu();

		void updateSize(glm::ivec2 size) override;

		void update() override;
		void render() override;

		void start() override;
		void end() override;

		WorldScene();
	};
}