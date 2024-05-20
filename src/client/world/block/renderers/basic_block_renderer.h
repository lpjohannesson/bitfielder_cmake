#pragma once
#include "block_renderer.h"

namespace bf {
    class BasicBlockRenderer : public BlockRenderer {
    public:
        Sprite sprite;

        void render(const BlockRenderData &renderData) override;
    };
}