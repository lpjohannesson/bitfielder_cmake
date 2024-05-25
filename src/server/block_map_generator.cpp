#include "block_map_generator.h"

using namespace bf;

void BlockMapGenerator::generateChunk(World &world, BlockChunk &chunk) {
    for (int y = 2; y < BlockChunk::SIZE.y; y++) {
        for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            glm::ivec2 position = { x, y };
            
            chunk.setBlockIndex(position, rand() % 2);
        }
    }
}