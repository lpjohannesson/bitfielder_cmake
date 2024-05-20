#pragma once
#include <entt/entt.hpp>

namespace bf {
    class World;

    class Content {
    public:
        entt::entity airBlock, testBlock;

        void loadContent(World &world);
    };
}