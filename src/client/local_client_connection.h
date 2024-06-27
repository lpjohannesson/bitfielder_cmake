#pragma once
#include "server/client_connection.h"
#include "scenes/world_scene.h"

namespace bf {
	class LocalClientConnection : public ClientConnection {
	public:
        WorldScene *scene;

        void writePacket(Packet &packet) override;
	};
}