#pragma once
#include "client_connection.h"
#include "world/world.h"
#include "block_map_generator.h"

namespace bf {
	class Server {
    private:
        std::vector<ClientConnection*> clients;

	public:
        World world;
        BlockMapGenerator mapGenerator;

        void addClient(ClientConnection *client);

        void sendChunk(ClientConnection *client, BlockChunk *chunk);

        Server();
	};
}