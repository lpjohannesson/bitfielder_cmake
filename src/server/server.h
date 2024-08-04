#pragma once
#include "client_connection.h"
#include "world/world.h"
#include "block_map_generator.h"

namespace bf {
	class Server {
        private:
                entt::registry *entityRegistry;

        public:
                std::vector<ClientConnection*> clients;

                World world;
                BlockMapGenerator mapGenerator;

                void broadcastPacket(Packet &packet, ClientConnection *client);

                void addClient(ClientConnection *client);
                void removeClient(ClientConnection *client);
                
                void writePlayerState(Packet &packet, entt::entity player);
                void writeBlockChunk(Packet &packet, BlockChunk &chunk);
                void writeReplaceBlock(Packet &packet, glm::ivec2 position, bool onFrontLayer, int blockIndex);
                void writeDespawnEntity(Packet &packet, int playerID);
                void writeEntityPosition(Packet &packet, entt::entity entity);
                void writeEntitySpriteAnimation(Packet &packet, entt::entity entity);
                void writeEntitySpriteFlip(Packet &packet, entt::entity entity);
                void writeEntitySpriteAim(Packet &packet, entt::entity entity);
                void writeRemotePlayer(Packet &packet, entt::entity player);

                void readPlayerState(ClientConnection *client, Packet &packet);
                void readReplaceBlock(ClientConnection *client, Packet &packet);

                void readPacket(ClientConnection *client, Packet &packet);

                Server();
	};
}