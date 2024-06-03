#include "block_map_generator.h"

using namespace bf;

void BlockMapGenerator::generateChunk(World &world, BlockChunk &chunk) {
    // TODO: Add system for block type IDs
    for (int x = 0; x < BlockChunk::SIZE.x; x++) {
        BlockData *blockData = chunk.getBlock({ x, 15 });
        
        blockData->frontIndex = 1;
    }
}