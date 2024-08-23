#pragma once
#include "../block_render_data.h"
#include "../components/block_basic_renderer_component.h"

namespace bf {
    class BasicBlockRenderer {
    public:
        static void render(const BlockRenderData &data);
        static void createBlock(const BlockRendererFactoryData &data);
    };
}