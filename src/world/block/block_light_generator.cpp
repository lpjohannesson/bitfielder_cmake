#include "block_light_generator.h"

using namespace bf;

void BlockLightGenerator::queueNeighboringChunk(BlockChunk *chunk, int x, std::queue<BlockLightCell> &cellQueue) {
    if (chunk == nullptr) {
        return;
    }

    int blockStartX = chunk->getMapIndex() * BlockChunk::SIZE.x;

    for (int y = 0; y < BlockChunk::SIZE.y; y++) {
        BlockData *blockData = chunk->getBlock({ x, y });

        cellQueue.push({ { blockStartX + x, y }, blockData->light });
        blockData->light = 0;
    }
}

void BlockLightGenerator::removeLight(glm::ivec2 position, BlockMap<BlockChunk> &map) {
    
}

void BlockLightGenerator::spreadLight(BlockMap<BlockChunk> &map, std::queue<BlockLightCell> &cellQueue) {
    // TODO: Chunk optimizations

    while (!cellQueue.empty()) {
        BlockLightCell cell = cellQueue.front();
        cellQueue.pop();

        BlockData *blockData = BlockChunk::getWorldBlock(map, cell.position);

        if (blockData == nullptr) {
            continue;
        }

        // Skip if already bright enough
        if (cell.light <= blockData->light) {
            continue;
        }

        blockData->light = cell.light;

        if (cell.light == MAX_LIGHT) {
            // Fill below fully if on air
            int nextBelowLight;

            if (blockData->frontIndex == 0) {
                nextBelowLight = MAX_LIGHT;
            }
            else {
                nextBelowLight = MAX_LIGHT - LIGHT_STEP;
            }

            cellQueue.push({ cell.position + glm::ivec2(0, 1), nextBelowLight });

            // Fill sides
            for (glm::ivec2 offset : sideOffsets) {
                cellQueue.push({ cell.position + offset, MAX_LIGHT - LIGHT_STEP });
            }
        }
        else {
            // Always reduce light
            int nextLight = cell.light - LIGHT_STEP;

            if (nextLight <= 0) {
                continue;
            }

            for (glm::ivec2 offset : offsets) {
                cellQueue.push({ cell.position + offset, nextLight });
            }
        }
    }
}

void BlockLightGenerator::generateChunk(BlockChunk &chunk, BlockMap<BlockChunk> &map) {
    int blockStartX = chunk.getMapIndex() * BlockChunk::SIZE.x;
    
    std::queue<BlockLightCell> cellQueue;

    // Re-queue neighbours
    int mapIndex = chunk.getMapIndex();

    queueNeighboringChunk(map.getChunk(mapIndex - 1), BlockChunk::SIZE.x - 1, cellQueue);
    queueNeighboringChunk(map.getChunk(mapIndex + 1), 0, cellQueue);

    // Fill top with full sunlight
    for (int x = 0; x < BlockChunk::SIZE.x; x++) {
        cellQueue.push({ { blockStartX + x, 0 }, MAX_LIGHT });
    }

    spreadLight(map, cellQueue);
}