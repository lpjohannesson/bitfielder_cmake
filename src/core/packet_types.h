#pragma once

namespace bf {
	enum class ClientPacket {
        PLAYER_POSITION
	};

    enum class ServerPacket {
        BLOCK_CHUNK,
        DESPAWN_ENTITY,
        ENTITY_POSITION,
        REMOTE_PLAYER
	};
}