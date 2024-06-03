#include "block_map_generator.h"

using namespace bf;

void BlockMapGenerator::generateChunk(World &world, BlockChunk &chunk) {
    // TODO: Add system for block type IDs
    for (int x = 0; x < BlockChunk::SIZE.x; x++) {
        chunk.getBlock({ x, 15 })->frontIndex = 1;
        chunk.getBlock({ x, 14 })->frontIndex = 2;
        chunk.getBlock({ x, 13 })->frontIndex = 3;
        chunk.getBlock({ x, 12 })->frontIndex = 4;
    }
}