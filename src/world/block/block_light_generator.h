#pragma once
#include <queue>
#include <glm/glm.hpp>
#include "block_chunk.h"

namespace bf {
    struct BlockLightCell {
        glm::ivec2 position;
        int light;
    };

    class BlockLightGenerator {
    private:
        static constexpr glm::ivec2
            offsets[] = {
                { -1, 0 },
                { 1, 0 },
                { 0, -1 },
                { 0, 1 }
            },
            sunOffsets[] {
                { -1, 0 },
                { 1, 0 },
                { 0, 1 }
            };

    public:
        static constexpr int MAX_LIGHT = 15;

        static void spreadLight(BlockMap<BlockChunk> &map, std::queue<BlockLightCell> cellQueue);
        static void generateChunk(BlockChunk &chunk, BlockMap<BlockChunk> &map);
    };
}