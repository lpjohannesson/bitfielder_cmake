#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "entity/systems/body_system.h"

namespace bf {
    class World;

    class Content {
    public:
        BodySystem bodySystem;

        entt::entity airBlock, dirtBlock, grassBlock, woodBlock, goldBlock, woolBlock;

        void createPlayer(entt::entity player, World &world);

        Content(World &world);
    };
}