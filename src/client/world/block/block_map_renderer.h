#pragma once
#include "world/block/block_map.h"
#include "world/block/block_chunk.h"
#include "client/world/block/block_mesh.h"
#include "gfx/sprite/sprite_batch.h"

namespace bf {
    class World;
    class WorldScene;

    class BlockMapRenderer {
    public:
        BlockMap<BlockMesh> map;
        SpriteBatch spriteBatch;

        void createMesh(const World &world, BlockChunk &chunk);
        void render(const WorldScene &scene);
    };
}