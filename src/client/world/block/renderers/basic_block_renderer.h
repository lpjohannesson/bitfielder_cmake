#pragma once
#include "block_renderer.h"

namespace bf {
    class BasicBlockRenderer : public BlockRenderer {
    public:
        void render(const BlockRenderData &renderData) override;
        
        BasicBlockRenderer(const rapidjson::Value &value, entt::entity block, WorldScene &scene);
    };
}