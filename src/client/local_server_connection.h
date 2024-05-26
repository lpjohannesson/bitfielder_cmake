#pragma once
#include "server_connection.h"

namespace bf {
    class LocalWorldScene;

	class LocalServerConnection : public ServerConnection {
    private:
        LocalWorldScene *worldScene;

	public:
        void writePacket(Packet &packet) override;

        LocalServerConnection(LocalWorldScene *worldScene);
	};
}