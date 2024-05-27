#pragma once
#include "client_connection.h"
#include "world/world.h"
#include "block_map_generator.h"

namespace bf {
	class Server {
	public:
                std::vector<ClientConnection*> clients;

                World world;
                BlockMapGenerator mapGenerator;

                void addClient(ClientConnection *client);
                void removeClient(ClientConnection *client);

                void writeBlockChunk(ClientConnection *client, BlockChunk *chunk);
                void writeDespawnEntity(ClientConnection *client, int playerID);
                void writeEntityPosition(ClientConnection *client, entt::entity entity);
                void writeRemotePlayer(ClientConnection *client, entt::entity player);

                void readPacket(ClientConnection *client, Packet &packet);

                Server();
	};
}