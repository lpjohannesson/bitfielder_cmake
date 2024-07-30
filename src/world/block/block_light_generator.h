#pragma once
#include <queue>
#include <glm/glm.hpp>
#include "block_chunk.h"
#include "block_map.h"

namespace bf {
    struct BlockLightCell {
        glm::ivec2 position;
        int light;
        BlockData *blockData;
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

        static void queueNeighboringChunk(BlockChunk *chunk, int x, std::queue<BlockLightCell> &cellQueue);

    public:
        static constexpr int MAX_LIGHT = 15;

        static void removeLight(glm::ivec2 position, BlockMap<BlockChunk> &map);

        static void spreadLight(BlockMap<BlockChunk> &map, std::queue<BlockLightCell> &cellQueue);
        static void generateChunk(BlockChunk &chunk, BlockMap<BlockChunk> &map);
    };
}