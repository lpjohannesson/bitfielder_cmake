#pragma once
#include <enet/enet.h>
#include <memory>
#include "world_scene.h"
#include "server/server.h"
#include "client/remote_server_connection.h"

namespace bf {
	class RemoteWorldScene : public WorldScene {
	public:
		ENetHost *networkHost;
		ENetAddress networkAddress;
		ENetPeer *networkServer;

		RemoteServerConnection remoteServerConnection;

		void update() override;

		void start() override;

		RemoteWorldScene(const char *ip, int port);
		~RemoteWorldScene();
	};
}