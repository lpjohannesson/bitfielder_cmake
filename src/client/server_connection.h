#pragma once
#include "client_packet_manager.h"

namespace bf {
    class WorldScene;

	class ServerConnection {
    public:
        inline virtual bool host(WorldScene &scene) = 0;
        inline virtual void writePacket(Packet &packet) {}

        inline virtual void end() {}

        inline virtual ~ServerConnection() {}
	};
}