#pragma once
#include "world/block/block_map.h"
#include "world/block/block_chunk.h"
#include "block_mesh.h"
#include "renderers/block_renderer.h"
#include "gfx/sprite/sprite_batch.h"

namespace bf {
    class WorldScene;

    class BlockMapRenderer {
    private:
        void renderBlock(const BlockRenderData &renderData);

    public:
        BlockMap<BlockMesh> map;
        SpriteBatch frontSpriteBatch, backSpriteBatch;

        void createMesh(WorldScene &scene, const BlockChunk &chunk);
    };
}