#pragma once

namespace bf {
        enum class ClientPacket {
                PLAYER_POSITION,
                PLAYER_SPRITE_ANIMATION,
                PLAYER_SPRITE_FLIP
        };

        enum class ServerPacket {
                BLOCK_CHUNK,
                DESPAWN_ENTITY,
                ENTITY_POSITION,
                ENTITY_SPRITE_ANIMATION,
                ENTITY_SPRITE_FLIP,
                REMOTE_PLAYER
        };
}