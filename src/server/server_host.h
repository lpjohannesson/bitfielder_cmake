#pragma once
#include <vector>
#include <enet/enet.h>
#include "server.h"
#include "remote_client_connection.h"

namespace bf {
	class ServerHost {
    private:
        static std::string getCin();

    public:
        std::vector<RemoteClientConnection*> remoteClients;

        Server server;

        ENetHost *networkHost;
		ENetAddress networkAddress;

        void disconnectClient(RemoteClientConnection *client);

        ServerHost(int port);
        ~ServerHost();
	};
}