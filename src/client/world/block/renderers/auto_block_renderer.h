#pragma once
#include "block_renderer.h"
#include "gfx/sprite/sprite.h"
#include "gfx/sprite/sprite_frames.h"

namespace bf {
    class AutoBlockRenderer : public BlockRenderer {
    private:
        static constexpr int 
            SHIFT_HORIZONTAL = 0,
            SHIFT_VERTICAL = 1,
            SHIFT_DIAGONAL = 2;
        
        static constexpr int frameStartLookup[] { 0, 4, 2, 6, 0, 4, 2, 8 };

        static int checkNeighbor(const BlockRenderData &renderData, glm::ivec2 offset);
        void drawCorner(const BlockRenderData &renderData, int frame, glm::vec2 offset, Sprite &sprite, SpriteFrames &frames);

    public:
        void render(const BlockRenderData &renderData) override;

        AutoBlockRenderer(const rapidjson::Value &value, entt::entity block, WorldScene &scene);
    };
}