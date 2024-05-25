#include "block_map.h"

using namespace bf;

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