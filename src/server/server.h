#pragma once
#include "client_connection.h"
#include "world/world.h"
#include "block_map_generator.h"
#include "core/random.h"

namespace bf {
	class Server {
        public:
                std::vector<ClientConnection*> clients;

                World world;
                BlockMapGenerator mapGenerator;

                Random random;

                void broadcastPacket(Packet &packet, ClientConnection *client);

                void addClient(ClientConnection *client);
                void removeClient(ClientConnection *client);
                
                void writePlayerState(Packet &packet, entt::entity player);

                void writeTeleportPlayer(Packet &packet, ClientConnection *client, glm::vec2 position);
                void writeChunkData(Packet &packet, BlockChunk &chunk);
                void writeReplaceBlock(Packet &packet, glm::ivec2 position, bool onFrontLayer, int blockIndex);
                void writeDespawnEntity(Packet &packet, int playerID);
                void writeEntityPosition(Packet &packet, entt::entity entity);
                void writeEntityAnimation(Packet &packet, entt::entity entity);
                void writeEntityFlip(Packet &packet, entt::entity entity);
                void writeEntityAim(Packet &packet, entt::entity entity);
                void writeRemotePlayer(Packet &packet, entt::entity player);

                void readPlayerState(ClientConnection *client, Packet &packet);
                void readReplaceBlock(ClientConnection *client, Packet &packet);

                void readPacket(ClientConnection *client, Packet &packet);

                Server();
                ~Server();
	};
}