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

    class BlockLightGenerator {
    private:
        static constexpr int LIGHT_STEP = 1;

        static constexpr glm::ivec2
            offsets[] = {
                { 0, -1 },
                { -1, 0 },
                { 1, 0 },
                { 0, 1 },
            },
            sideOffsets[] = {
                { -1, 0 },
                { 1, 0 },
            };

        static bool isBlockOpaque(BlockData &blockData, World &world);

        static void queueNeighboringChunk(int x, BlockChunk *chunk, std::queue<BlockLightCell> &cellQueue);

        static void spreadSunlight(std::queue<BlockLightCell> &cellQueue, World &world, Box2i &resultBox);
        static void updateSunlight(glm::ivec2 position, World &world, Box2i &resultBox);

    public:
        static constexpr int MAX_LIGHT = 15;

        static void updateLight(glm::ivec2 position, World &world, Box2i &resultBox);

        static void generateChunk(BlockChunk &chunk, World &world);
    };
}