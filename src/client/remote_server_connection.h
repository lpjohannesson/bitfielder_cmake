#pragma once
#include <enet/enet.h>
#include "server_connection.h"

namespace bf {
        class RemoteServerConnection : public ServerConnection {
        public:
                ENetPeer *networkPeer;

                void writePacket(Packet &packet) override;
        };
}