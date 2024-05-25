#pragma once
#include "core/packet.h"

namespace bf {
	class ClientConnection {
    public:
        inline virtual void writePacket(Packet &packet) {}
	};
}