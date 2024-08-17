#pragma once
#include <entt/entt.hpp>
#include <string>
#include "../renderer_factory.h"

namespace bf {
    class WorldScene;

    class BlockRendererFactory {
    public:
        static TextureSection getBlockTexture(const rapidjson::Value &value, const WorldScene &scene);
        static TextureSection getParticleTexture(const rapidjson::Value &value, const WorldScene &scene);

        static void createParticle(const std::string name, entt::entity block, WorldScene &scene);
        static void createBlock(entt::entity block, WorldScene &scene);
    
        static void start(WorldScene &scene);
        static void end(WorldScene &scene);
    };
}