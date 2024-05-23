#pragma once
#include "block_renderer.h"
#include "gfx/sprite/sprite.h"

namespace bf {
    class BasicBlockRenderer : public BlockRenderer {
    public:
        Sprite sprite;

        void render(const BlockRenderData &renderData) override;
    };
}