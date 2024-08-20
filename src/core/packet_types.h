#pragma once

namespace bf {
        enum class ClientPacket {
                PLAYER_STATE,
                REPLACE_BLOCK
        };

        enum class ServerPacket {
                TELEPORT_PLAYER,
                CHUNK_DATA,
                REPLACE_BLOCK,
                DESPAWN_ENTITY,
                ENTITY_POSITION,
                ENTITY_SPRITE_ANIMATION,
                ENTITY_SPRITE_FLIP,
                ENTITY_SPRITE_AIM,
                REMOTE_PLAYER
        };
}