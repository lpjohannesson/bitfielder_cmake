#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <rapidjson/document.h>
#include "gfx/sprite/sprite_batch.h"
#include "world/block/block_sample.h"
#include "world/block/block_chunk.h"

namespace bf {
    class WorldScene;
    class BlockMapRenderer;

    class BlockRenderData {
    public:
        WorldScene *scene;
        BlockMapRenderer *renderer;
        SpriteBatch *spriteBatch;
        BlockSample<BlockChunk> *blockSample;

        int blockIndex;
        glm::ivec2 position;
        bool onFrontLayer;
    };

    class BlockRenderer {
    public:
        inline virtual void render(const BlockRenderData &renderData) {}
    };
}