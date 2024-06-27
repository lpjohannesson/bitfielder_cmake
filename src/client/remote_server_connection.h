#pragma once
#include <string>
#include <enet/enet.h>
#include "server_connection.h"

namespace bf {

        class RemoteServerConnection : public ServerConnection {
        public:
                ENetPeer *networkPeer;
                ENetHost *networkHost;
		ENetAddress networkAddress;

                bool startConnection(const char *ip, int port, std::string &error);
                void endConnection();

                bool updateConnection();

                bool host(WorldScene &scene) override;
                void writePacket(Packet &packet) override;

                void end() override;
        };
}