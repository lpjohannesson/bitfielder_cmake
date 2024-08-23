#pragma once
#include "../block_render_data.h"
#include "gfx/sprite/sprite.h"
#include "gfx/sprite/sprite_frames.h"
#include "../components/block_auto_renderer_component.h"

namespace bf {
    class AutoBlockRenderer {
    private:
        static constexpr int 
            SHIFT_HORIZONTAL = 0,
            SHIFT_VERTICAL = 1,
            SHIFT_DIAGONAL = 2;
        
        static constexpr int frameStartLookup[] { 0, 4, 2, 6, 0, 4, 2, 8 };

        static int checkNeighbor(const BlockRenderData &data, glm::ivec2 offset);
        static void drawCorner(const BlockRenderData &data, int frame, glm::vec2 offset, SpriteFrames &frames);

    public:
        static void render(const BlockRenderData &data);
        static void createBlock(const BlockRendererFactoryData &data);
    };
}