#pragma once
#include "core/packet.h"

namespace bf {
	class ServerConnection {
    public:
        inline virtual void writePacket(Packet &packet) {}
	};
}