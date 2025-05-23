#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <rapidjson/document.h>
#include "gfx/sprite/sprite_batch.h"
#include "world/block/block_sample.h"
#include "world/block/block_chunk.h"

namespace bf {
    class WorldScene;

    class BlockRenderData {
    public:
        WorldScene &scene;
        BlockSample<BlockChunk> &blockSample;

        SpriteBatch *spriteBatch;
        
        glm::ivec2 position;
        int blockIndex;
        bool onFrontLayer;
    };

    class BlockRendererFactoryData {
    public:
        rapidjson::Value &value;
        WorldScene &scene;
        entt::entity block;
    };
}