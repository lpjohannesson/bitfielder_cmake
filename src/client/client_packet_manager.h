#pragma once
#include <entt/entt.hpp>
#include "core/packet.h"
#include "world/block/block_chunk.h"

namespace bf {
    class WorldScene;

	class ClientPacketManager {
	public:
        static void writePlayerState(WorldScene &scene);
		static void writeReplaceBlock(glm::ivec2 position, bool onFrontLayer, BlockData &blockData, WorldScene &scene);

		static bool readEntityPacket(entt::entity &entity, Packet &packet, WorldScene &scene);

		static void readTeleportPlayer(Packet &packet, WorldScene &scene);
		static void readChunkData(Packet &packet, WorldScene &scene);
		static void readReplaceBlock(Packet &packet, WorldScene &scene);
		static void readDespawnEntity(Packet &packet, WorldScene &scene);
		static void readEntityPosition(Packet &packet, WorldScene &scene);
		static void readEntitySpriteAnimation(Packet &packet, WorldScene &scene);
		static void readEntitySpriteFlip(Packet &packet, WorldScene &scene);
		static void readEntitySpriteAim(Packet &packet, WorldScene &scene);
		static void readRemotePlayer(Packet &packet, WorldScene &scene);

		static void readPacket(Packet &packet, WorldScene &scene);
	};
}