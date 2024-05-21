#pragma once
#include <vector>
#include "block_mesh.h"
#include "world/world.h"
#include "renderers/block_renderer.h"
#include "gfx/sprite/sprite_batch.h"

namespace bf {
    class WorldRenderer;

    class BlockMapRenderer {
    public:
        SpriteBatch spriteBatch;

        void createMesh(const WorldRenderer &renderer, const World &world, BlockChunk &chunk);
        void render(const WorldRenderer &renderer, const World &world);
    };
}