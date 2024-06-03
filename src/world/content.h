#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "entity/systems/body_system.h"

namespace bf {
    class World;

    struct PlayerSpawnProperties {
        glm::vec2 position{};
        int spriteAnimationIndex = 0;
        bool spriteFlipX = false;
    };

    class Content {
    public:
        BodySystem bodySystem;

        entt::entity airBlock, dirtBlock, woodBlock, goldBlock, woolBlock;

        void createPlayer(entt::entity player, World &world, const PlayerSpawnProperties &spawnProperties);

        Content(World &world);
    };
}