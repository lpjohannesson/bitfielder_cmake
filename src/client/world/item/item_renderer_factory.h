#pragma once
#include <entt/entt.hpp>
#include "../renderer_factory.h"

namespace bf {
    class WorldScene;

    class ItemRendererFactory {
    public:
        static TextureSection getItemTexture(const rapidjson::Value &value, const WorldScene &scene);

        static void createItem(entt::entity item, WorldScene &scene);

        static void start(WorldScene &scene);
    };
}