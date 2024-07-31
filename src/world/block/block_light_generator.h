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
        static constexpr int LIGHT_STEP = 2;

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

        static bool isBlockOpaque(BlockData *blockData, World &world);

        static void queueNeighboringChunk(BlockChunk *chunk, int x, std::queue<BlockLightCell> &cellQueue);

    public:
        static constexpr int MAX_LIGHT = 15;

        static void removeLight(glm::ivec2 position, World &world);

        static void spreadLight(std::queue<BlockLightCell> &cellQueue, World &world);
        static void generateChunk(BlockChunk &chunk, World &world);
    };
}