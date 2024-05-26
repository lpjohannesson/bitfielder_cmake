#pragma once
#include <unordered_map>
#include <list>
#include <enet/enet.h>
#include "server.h"
#include "remote_client_connection.h"

namespace bf {
	class ServerHost {
    public:
        std::list<RemoteClientConnection> remoteClients;
        std::unordered_map<ENetPeer*, ClientConnection*> peerClients;
        Server server;

        ENetHost *networkHost;
		ENetAddress networkAddress;

        ClientConnection *getRemoteClient(ENetPeer *peer);

        ServerHost(int port);
        ~ServerHost();
	};
}