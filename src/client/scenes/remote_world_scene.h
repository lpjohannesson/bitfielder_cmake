#pragma once
#include <enet/enet.h>
#include "world_scene.h"
#include "server/server.h"
#include "client/local_client_connection.h"

namespace bf {
	class RemoteWorldScene : public WorldScene {
	public:
		ENetHost *networkHost;
		ENetAddress networkAddress;
		ENetPeer *networkServer;

		void update() override;

		RemoteWorldScene(const char *ip, int port);
		~RemoteWorldScene();
	};
}