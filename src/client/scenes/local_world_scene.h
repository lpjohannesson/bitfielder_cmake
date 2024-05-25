#pragma once
#include "world_scene.h"
#include "server/server.h"
#include "client/local_client_connection.h"

namespace bf {
	class LocalWorldScene : public WorldScene {
	public:
		Server server;
		LocalClientConnection localClientConnection;
        
        void start() override;

        inline LocalWorldScene() : localClientConnection(this) {}
	};
}