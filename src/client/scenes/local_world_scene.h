#pragma once
#include "world_scene.h"
#include "server/server.h"
#include "client/local_client_connection.h"
#include "client/local_server_connection.h"

namespace bf {
	class LocalWorldScene : public WorldScene {
	public:
		Server localServer;
		LocalClientConnection localClientConnection;
		LocalServerConnection localServerConnection;
        
        void start() override;
		void end() override;

        inline LocalWorldScene() : localClientConnection(this), localServerConnection(this) {}
	};
}