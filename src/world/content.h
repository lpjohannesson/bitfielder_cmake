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

        void createPlayer(entt::entity player, World &world, glm::vec2 position);

        Content(World &world);
    };
}