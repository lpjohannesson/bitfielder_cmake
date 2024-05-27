#pragma once
#include <enet/enet.h>
#include "server.h"

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