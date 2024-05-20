#pragma once
#include <glm/glm.hpp>

namespace bf {
    class BlockMapRenderer;

    class BlockRenderData {
    public:
        BlockMapRenderer *renderer;

        glm::vec2 renderPosition;
    };

    class BlockRenderer {
    public:
        inline virtual void render(const BlockRenderData &renderData) {}
    };
}

#include "client/world/block/block_map_renderer.h"