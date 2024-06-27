#pragma once
#include "server_connection.h"
#include "server/server.h"
#include "local_client_connection.h"

namespace bf {
	class LocalServerConnection : public ServerConnection {
	public:
        Server server;
        LocalClientConnection clientConnection;

        void startScene(WorldScene &scene);

        bool host(WorldScene &scene) override;
        void writePacket(Packet &packet) override;

        void end() override;
	};
}