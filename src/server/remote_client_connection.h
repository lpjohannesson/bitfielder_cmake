#pragma once
#include <enet/enet.h>
#include "client_connection.h"

namespace bf {
        class RemoteClientConnection : public ClientConnection {
        public:
                ENetPeer *networkPeer;

                void writePacket(Packet &packet) override;
        };
}