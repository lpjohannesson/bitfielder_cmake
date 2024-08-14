#pragma once
#include <entt/entt.hpp>

namespace bf {
    class World;

    class ItemContent {
    public:
        entt::entity
            dirt,
            stone;

        ItemContent(World &world);
    };
}