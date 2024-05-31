#pragma once
#include <glm/glm.hpp>
#include "gfx/sprite/sprite_batch.h"

namespace bf {
    class BlockMapRenderer;

    class BlockRenderData {
    public:
        BlockMapRenderer *renderer;
        SpriteBatch *spriteBatch;

        int blockIndex;
        glm::ivec2 position;
    };

    class BlockRenderer {
    public:
        inline virtual void render(const BlockRenderData &renderData) {}
    };
}