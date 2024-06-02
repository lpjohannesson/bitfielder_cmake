#include "block_map_generator.h"

using namespace bf;

void BlockMapGenerator::generateChunk(World &world, BlockChunk &chunk) {
    // TODO: Add system for block type IDs
    srand(time(NULL));
    
    for (int y = 6; y < BlockChunk::SIZE.y; y++) {
        for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            BlockData *blockData = chunk.getBlock({ x, y });
            
            blockData->backIndex = 1;
        }
    }

    for (int y = 8; y < BlockChunk::SIZE.y; y++) {
        for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            BlockData *blockData = chunk.getBlock({ x, y });
            
            blockData->frontIndex = 1;
        }
    }
}