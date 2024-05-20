#include "block_map.h"

using namespace bf;

entt::entity *BlockMap::getChunk(int index) const {
    // Find chunk by index
    auto foundChunk = chunkMap.find(index);

    if (foundChunk == chunkMap.end()) {
        return nullptr;
    }

    return (entt::entity*)foundChunk->second;
}

entt::entity BlockMap::createChunk(int index) {
    entt::entity chunk = registry.create();
    registry.emplace<BlockChunk*>(chunk, new BlockChunk());

    chunkMap.emplace(index, chunk);
    return chunk;
}