#include "block_map.h"
#include <glm/glm.hpp>

using namespace bf;

int BlockMap::getChunkIndex(int blockX) {
    return glm::floor(blockX / (float)BlockChunk::SIZE.x);
}

glm::ivec2 BlockMap::worldToChunk(glm::ivec2 position, int chunkIndex) {
    return { position.x - chunkIndex * BlockChunk::SIZE.x, position.y };  
}

BlockChunk *BlockMap::getChunk(int index) const {
    // Find chunk by index
    auto foundChunk = chunks.find(index);

    if (foundChunk == chunks.end()) {
        return nullptr;
    }

    return (BlockChunk*)&foundChunk->second;
}

BlockChunk &BlockMap::createChunk(int index) {
    // Construct chunk with index parameter
    chunks.emplace(index, index);
    return chunks.at(index);
}