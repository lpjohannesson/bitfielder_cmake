#pragma once
#include <entt/entt.hpp>

namespace bf {
    class World;

    class ItemContent {
    public:
        entt::entity
            dirt,
            stone,
            woodLog,
            woodPlanks,
            stick,
            leaves,
            bush,
            mushroom,
            wheat,
            torch,
            iron;

        void createBlockItem(entt::entity item, entt::entity block, World &world);

        ItemContent(World &world);
    };
}