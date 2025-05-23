#pragma once
#include <entt/entt.hpp>
#include "core/packet.h"

namespace bf {
	class ClientConnection {
    public:
        entt::entity player;

        inline virtual void writePacket(Packet &packet) {}
	};
}