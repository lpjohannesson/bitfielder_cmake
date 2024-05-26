#pragma once
#include <unordered_map>
#include <enet/enet.h>
#include "server.h"

namespace bf {
	class ServerHost {
    public:
        std::unordered_map<ENetPeer*, ClientConnection*> clients;
        Server server;

        ENetHost *networkHost;
		ENetAddress networkAddress;

        ServerHost(int port);
        ~ServerHost();
	};
}