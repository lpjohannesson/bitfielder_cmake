#pragma once
#include <entt/entt.hpp>
#include "entity/systems/body_system.h"

namespace bf {
    class World;

    class Content {
    public:
        BodySystem bodySystem;

        entt::entity player;

        entt::entity airBlock, testBlock;

        void loadContent(World &world);
    };
}