#pragma once
#include <string>
#include <rapidjson/document.h>
#include <entt/entt.hpp>
#include "gfx/core/texture_atlas.h"
#include "renderers/block_renderer.h"
#include "core/box2.h"

namespace bf {
    class WorldScene;

    class BlockRendererFactory {
    private:
        static TextureSection getRendererTexture(const WorldScene &scene, const rapidjson::Value &value, std::string basePath);
        static TextureSection getBlockTexture(const WorldScene &scene, const rapidjson::Value &value);
        static TextureSection getParticleTexture(const WorldScene &scene, const rapidjson::Value &value);

        static void createParticleRenderer(WorldScene &scene, entt::entity block, const rapidjson::Value &blockValue, std::string particleName);

        static BlockRenderer *createBasicBlockRenderer(const WorldScene &scene, const rapidjson::Value &value);
        static BlockRenderer *createAutoBlockRenderer(const WorldScene &scene, const rapidjson::Value &value);

        static void createBlockRenderer(WorldScene &scene, entt::entity block);

    public:
        static void createRenderers(WorldScene &scene);
        static void destroyRenderers(WorldScene &scene);
    };
}