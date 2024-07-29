#include "block_light_generator.h"
#include "block_sample.h"

using namespace bf;

void BlockLightGenerator::spreadLight(BlockMap<BlockChunk> &map, std::queue<BlockLightCell> cellQueue) {
    BlockChunk *chunk = nullptr;
    int lastChunkIndex;

    // Loop through cell queue
    while (!cellQueue.empty()) {
        BlockLightCell cell = cellQueue.front();
        cellQueue.pop();

        // Spread to neighbours
        if (cell.light <= MAX_LIGHT) {

        }
        else {
            for (glm::ivec2 offset : sunOffsets) {
                glm::ivec2 position = cell.position + offset;

                // Get new chunk or reuse
                int chunkIndex = BlockChunk::getChunkIndex(position.x);

                if (chunk == nullptr || chunkIndex != lastChunkIndex) {
                    chunk = map.getChunk(chunkIndex);

                    if (chunk == nullptr) {
                        continue;
                    }

                    lastChunkIndex = chunkIndex;
                }

                // Get block with vertical check
                glm::vec2 chunkPosition = BlockChunk::worldToChunk(position, chunkIndex);
                BlockData *blockData = chunk->getBlockChecked(chunkPosition);

                if (blockData == nullptr) {
                    continue;
                }

                // Skip if already lit
                if (blockData->light == MAX_LIGHT) {
                    return;
                }

                blockData->light = MAX_LIGHT;

                // Push new cell
                int newLight = 0;

                if (blockData->frontIndex == 0) {
                    newLight = MAX_LIGHT + 1;
                }
                else {
                    newLight = MAX_LIGHT;
                }

                cellQueue.push({ position, newLight });
            }
        }
    }
}

void BlockLightGenerator::generateChunk(BlockChunk &chunk, BlockMap<BlockChunk> &map) {
    int blockStartX = chunk.getMapIndex() * BlockChunk::SIZE.x;
    
    // Add full top sunlight to queue
    std::queue<BlockLightCell> cellQueue;

    for (int x = 0; x < BlockChunk::SIZE.x; x++) {
        cellQueue.push({ { blockStartX + x, 0 }, MAX_LIGHT + 1 });
    }

    spreadLight(map, cellQueue);
}