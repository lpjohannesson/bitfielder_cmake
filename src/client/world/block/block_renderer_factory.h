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
        static TextureSection getTexture(std::string basePath, const rapidjson::Value &value, const WorldScene &scene);
        
        static TextureSection getBlockTexture(const rapidjson::Value &value, const WorldScene &scene);
        static TextureSection getParticleTexture(const rapidjson::Value &value, const WorldScene &scene);

        static BlockParticleComponent *createParticleRenderer(const std::string name, entt::entity block, WorldScene &scene);
        static void createBlockRenderer(entt::entity block, WorldScene &scene);
    
        static void createRenderers(WorldScene &scene);
        static void destroyRenderers(WorldScene &scene);
    };
}