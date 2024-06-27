#pragma once
#include "core/packet.h"
#include "scenes/world_scene.h"

namespace bf {
	class ServerConnection {
    public:
        inline virtual bool host(WorldScene &scene) { return true; }
        inline virtual void writePacket(Packet &packet) {}

        inline virtual void end() {}

        inline virtual ~ServerConnection() {}
	};
}