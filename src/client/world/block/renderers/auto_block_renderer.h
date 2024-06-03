#pragma once
#include "block_renderer.h"
#include "gfx/sprite/sprite.h"
#include "gfx/sprite/animation/sprite_frames.h"

namespace bf {
    class AutoBlockRenderer : public BlockRenderer {
    private:
        static constexpr int bitmaskTable[] {
            11, 11, 31, 31, 11, 11, 31, 31, 30, 30, 21, 26, 30, 30, 21, 26,
            28, 28, 20, 20, 28, 28, 24, 24, 29, 29, 27, 40, 29, 29, 41, 25,
            11, 11, 31, 31, 11, 11, 31, 31, 30, 30, 21, 26, 30, 30, 21, 26,
            28, 28, 20, 20, 28, 28, 24, 24, 29, 29, 27, 40, 29, 29, 41, 25,
            15, 15, 23, 23, 15, 15, 23, 23, 13, 13, 19, 35, 13, 13, 19, 35,
            12, 12, 22, 22, 12, 12, 34, 34, 14, 14, 7, 47, 14, 14, 46, 01,
            15, 15, 23, 23, 15, 15, 23, 23, 10, 10, 43, 18, 10, 10, 43, 18,
            12, 12, 22, 22, 12, 12, 34, 34, 32, 32, 39, 3, 32, 32, 6, 45,
            11, 11, 31, 31, 11, 11, 31, 31, 30, 30, 21, 26, 30, 30, 21, 26,
            28, 28, 20, 20, 28, 28, 24, 24, 29, 29, 27, 40, 29, 29, 41, 25,
            11, 11, 31, 31, 11, 11, 31, 31, 30, 30, 21, 26, 30, 30, 21, 26,
            28, 28, 20, 20, 28, 28, 24, 24, 29, 29, 27, 40, 29, 29, 41, 25,
            15, 15, 23, 23, 15, 15, 23, 23, 13, 13, 19, 35, 13, 13, 19, 35,
            8, 8, 42, 42, 8, 8, 16, 16, 33, 33, 38, 5, 33, 33, 4, 44,
            15, 15, 23, 23, 15, 15, 23, 23, 10, 10, 43, 18, 10, 10, 43, 18,
            8, 8, 42, 42, 8, 8, 16, 16, 9, 9, 2, 37, 9, 9, 36, 17
        };

        static int checkNeighbor(const BlockRenderData &renderData, glm::ivec2 offset);
 
    public:
        Sprite sprite;
        SpriteFrames spriteFrames;

        void loadFrames(Box2 uvBox);

        void render(const BlockRenderData &renderData) override;
    };
}