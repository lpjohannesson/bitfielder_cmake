#pragma once
#include <string>
#include <rapidjson/document.h>
#include <entt/entt.hpp>
#include "core/box2.h"
#include "gfx/core/texture_atlas.h"
#include "renderers/block_renderer.h"
#include "components/block_particle_component.h"

namespace bf {
    class WorldScene;

    class BlockRendererFactory {
    public:
        static TextureSection getTexture(const std::string basePath, const rapidjson::Value &value, const WorldScene &scene);
        
        static TextureSection getBlockTexture(const rapidjson::Value &value, const WorldScene &scene);
        static TextureSection getParticleTexture(const rapidjson::Value &value, const WorldScene &scene);

        static void createParticle(const std::string name, entt::entity block, WorldScene &scene);
        static void createBlock(entt::entity block, WorldScene &scene);
    
        static void start(WorldScene &scene);
        static void end(WorldScene &scene);
    };
}