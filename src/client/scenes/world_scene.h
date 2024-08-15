#pragma once
#include <vector>
#include "engine/scene.h"
#include "world/world.h"
#include "core/packet.h"
#include "client/world/world_renderer.h"
#include "client/world/client_content.h"
#include "client/world/camera.h"
#include "client/menu/option_list.h"
#include "client/menu/text_list_option.h"
#include "client/world/entity/systems/client_entity_system.h"

namespace bf {
	class ServerConnection;

	class WorldScene : public Scene {
	private:
		entt::registry *entityRegistry;

	public:
		ServerConnection *server;

		World world;
		WorldRenderer worldRenderer;
		std::vector<ClientEntitySystem*> entitySystems;
		
		Camera camera;

		OptionList pauseOptionList;
		TextListOption pauseContinueOption, pauseTitleOption;

		OptionListRenderer optionListRenderer;

		SpriteMesh pauseLogoMesh;

		ClientContent clientContent;

		bool paused = false;

		void updateBlock(glm::ivec2 position);

		void spawnBlockParticles(glm::vec2 position, entt::entity block);

		void placeBlock(glm::ivec2 position, bool onFrontLayer, BlockData &blockData, int blockIndex);
		void destroyBlock(glm::ivec2 position, bool onFrontLayer, BlockData &blockData);

		void writePlayerState();
		void writeReplaceBlock(glm::ivec2 position, bool onFrontLayer, BlockData &blockData);

		bool readEntityPacket(Packet &packet, entt::entity &entity);

		void readBlockChunk(Packet &packet);
		void readReplaceBlock(Packet &packet);
		void readDespawnEntity(Packet &packet);
		void readEntityPosition(Packet &packet);
		void readEntitySpriteAnimation(Packet &packet);
		void readEntitySpriteFlip(Packet &packet);
		void readEntitySpriteAim(Packet &packet);
		void readRemotePlayer(Packet &packet);

		void readPacket(Packet &packet);

		bool updatePauseMenu();

		void updateSize(glm::ivec2 size) override;

		void update() override;
		void render() override;

		void start() override;
		void end() override;

		WorldScene();
	};
}