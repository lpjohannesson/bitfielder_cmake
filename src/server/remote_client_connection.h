#pragma once
#include <enet/enet.h>
#include "client_connection.h"

namespace bf {
	class RemoteClientConnection : public ClientConnection {
    private:
        ENetPeer *networkPeer;

	public:
        void writePacket(Packet &packet) override;

        RemoteClientConnection(ENetPeer *networkPeer);
	};
}