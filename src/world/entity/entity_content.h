#pragma once
#include <entt/entt.hpp>
#include "systems/body_system.h"

namespace bf {
    class World;

    class EntityContent {
    public:
        BodySystem bodySystem;

        void createPlayer(entt::entity player, World &world);

        EntityContent(World &world);
    };
}