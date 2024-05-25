#pragma once
#include "server/client_connection.h"

namespace bf {
    class WorldScene;

	class LocalClientConnection : public ClientConnection {
    private:
        WorldScene *worldScene;

	public:
        void writePacket(Packet &packet) override;

        LocalClientConnection(WorldScene *worldScene);
	};
}