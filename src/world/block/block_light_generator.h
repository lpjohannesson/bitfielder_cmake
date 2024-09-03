#pragma once
#include <queue>
#include <glm/glm.hpp>
#include "world/world.h"
#include "block_chunk.h"

namespace bf {
    struct BlockLightCell {
        glm::ivec2 position;
        int light;
    };

    class BlockLightQueue {
    public:
        std::queue<BlockLightCell> sun, red, green, blue;
    };

    class BlockLightGenerator {
    private:
        static constexpr int LIGHT_STEP = 1;

        static constexpr glm::ivec2
            offsets[] = {
                { 0, -1 },
                { -1, 0 },
                { 1, 0 },
                { 0, 1 },
            };

        static bool isBlockOpaque(BlockData &blockData, World &world);

        static void queueNeighboringChunk(int x, BlockChunk *chunk, BlockLightQueue &queue);

        static void spreadLight(BlockLightQueue &queue, World &world, Box2i &resultBox);
        static void updateSunlight(glm::ivec2 position, BlockLightQueue &queue, World &world, Box2i &resultBox);

    public:
        static constexpr int MAX_LIGHT = 15;

        static void updateLight(glm::ivec2 position, World &world, Box2i &resultBox);

        static void generateChunk(BlockChunk &chunk, World &world);
    };
}