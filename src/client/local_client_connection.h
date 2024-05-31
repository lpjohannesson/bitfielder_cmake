#pragma once
#include "server/client_connection.h"

namespace bf {
    class LocalWorldScene;

	class LocalClientConnection : public ClientConnection {
    private:
        LocalWorldScene *scene;

	public:
        void writePacket(Packet &packet) override;

        LocalClientConnection(LocalWorldScene *scene);
	};
}