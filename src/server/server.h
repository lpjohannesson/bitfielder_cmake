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

                void addClient(ClientConnection *client);
                void removeClient(ClientConnection *client);

                void writePlayerState(ClientConnection *client, entt::entity player);

                void writeBlockChunk(ClientConnection *client, BlockChunk *chunk);
                void writeReplaceBlock(ClientConnection *client, glm::ivec2 position, bool onFrontLayer, int blockIndex);
                void writeDespawnEntity(ClientConnection *client, int playerID);
                void writeEntityPosition(ClientConnection *client, entt::entity entity);
                void writeEntitySpriteAnimation(ClientConnection *client, entt::entity entity);
                void writeEntitySpriteFlip(ClientConnection *client, entt::entity entity);
                void writeEntitySpriteAim(ClientConnection *client, entt::entity entity);
                void writeRemotePlayer(ClientConnection *client, entt::entity player);

                void readPlayerState(ClientConnection *client, Packet &packet);
                void readReplaceBlock(ClientConnection *client, Packet &packet);

                void readPacket(ClientConnection *client, Packet &packet);

                Server();
	};
}