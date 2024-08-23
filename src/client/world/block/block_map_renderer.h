#pragma once
#include "gfx/sprite/sprite_batch.h"
#include "world/block/block_map.h"
#include "world/block/block_chunk.h"
#include "block_mesh.h"
#include "block_render_data.h"

namespace bf {
    class WorldScene;

    class BlockMapRenderer {
    private:
        void renderBlock(const BlockRenderData &data);

    public:
        BlockMap<BlockMesh> map;
        SpriteBatch frontSpriteBatch, backSpriteBatch, lightSpriteBatch;

        void createMesh(BlockChunk &chunk, WorldScene &scene, int sectionStart = 0, int sectionEnd = BlockMesh::SECTION_COUNT - 1);
    };
}