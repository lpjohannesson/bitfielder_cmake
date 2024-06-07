#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "entity/systems/body_system.h"

namespace bf {
    class World;

    class Content {
    public:
        BodySystem bodySystem;

        entt::entity
            airBlock,
            dirtBlock,
            grassBlock,
            stoneBlock,
            woodLogBlock,
            woodPlanksBlock,
            leavesBlock,
            bushBlock,
            mushroomBlock,
            ironBlockBlock,
            goldBlockBlock;

        void createPlayer(entt::entity player, World &world);

        Content(World &world);
    };
}