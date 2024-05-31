#include "block_map_generator.h"

using namespace bf;

void BlockMapGenerator::generateChunk(World &world, BlockChunk &chunk) {
    srand(time(NULL));
    for (int y = 2; y < BlockChunk::SIZE.y; y++) {
        for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            BlockData *blockData = chunk.getBlock({ x, y });
            
            blockData->frontIndex = rand() % (BlockChunk::SIZE.y - y) == 0;
            blockData->backIndex = rand() % glm::max(BlockChunk::SIZE.y - y - 4, 1) == 0;
        }
    }
}