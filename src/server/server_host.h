#pragma once
#include <unordered_map>
#include <enet/enet.h>
#include "server.h"
#include "remote_client_connection.h"

namespace bf {
	class ServerHost {
    public:
        Server server;

        ENetHost *networkHost;
		ENetAddress networkAddress;

        ServerHost(int port);
        ~ServerHost();
	};
}