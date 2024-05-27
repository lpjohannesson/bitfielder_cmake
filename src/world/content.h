#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "entity/systems/body_system.h"

namespace bf {
    class World;

    class Content {
    public:
        BodySystem bodySystem;

        entt::entity airBlock, testBlock;

        void spawnPlayer(entt::entity player, World &world, glm::vec2 position);

        void loadContent(World &world);
    };
}