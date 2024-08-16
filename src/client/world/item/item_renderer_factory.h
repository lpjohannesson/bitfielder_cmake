#pragma once
#include <entt/entt.hpp>

namespace bf {
    class WorldScene;

    class ItemRendererFactory {
    public:
        static void createItem(entt::entity item, WorldScene &scene);

        static void start(WorldScene &scene);
        static void end(WorldScene &scene);
    };
}