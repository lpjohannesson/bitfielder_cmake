#pragma once
#include "world/block/block_chunk.h"
#include "gfx/sprite/sprite_batch.h"

namespace bf {
    class World;
    class WorldScene;

    class BlockMapRenderer {
    public:
        SpriteBatch spriteBatch;

        void createMesh(const World &world, BlockChunk &chunk);
        void render(const WorldScene &scene);
    };
}