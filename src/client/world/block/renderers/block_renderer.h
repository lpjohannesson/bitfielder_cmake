#pragma once
#include <glm/glm.hpp>

namespace bf {
    class BlockMapRenderer;

    class BlockRenderData {
    public:
        BlockMapRenderer *renderer;

        glm::ivec2 position;
    };

    class BlockRenderer {
    public:
        inline virtual void render(const BlockRenderData &renderData) {}
    };
}

#include "client/world/block/block_map_renderer.h"