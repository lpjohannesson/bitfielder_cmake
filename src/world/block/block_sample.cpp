#include "block_sample.h"
#include <glm/glm.hpp>

using namespace bf;

BlockChunk* BlockSample::getChunk(int chunkIndex) const {
    return chunks[chunkIndex - chunkStart];
}

int BlockSample::getBlockIndex(glm::ivec2 position) const {
	int chunkIndex = BlockMap::getChunkIndex(position.x);
	BlockChunk* chunk = getChunk(chunkIndex);

	if (chunk == nullptr) {
		return 0;
	}

	glm::ivec2 chunkPosition = BlockMap::worldToChunk(position, chunkIndex);
	return chunk->getBlockIndex(chunkPosition);
}

int BlockSample::sampleBlockIndex(glm::ivec2 position) const {
	int chunkIndex = BlockMap::getChunkIndex(position.x);
	BlockChunk* chunk = getChunk(chunkIndex);

	if (chunk == nullptr) {
		return 0;
	}

	glm::ivec2 chunkPosition = BlockMap::worldToChunk(position, chunkIndex);
	return chunk->sampleBlockIndex(chunkPosition);
}

BlockSample::BlockSample(const BlockMap& blockMap, int blockStartX, int blockEndX) {
    // Get chunk extents
	chunkStart = BlockMap::getChunkIndex(blockStartX);
	int chunkEnd = BlockMap::getChunkIndex(blockEndX);
    int chunkCount = chunkEnd - chunkStart + 1;

    // Load chunks
	chunks.reserve(chunkCount);

	for (int i = 0; i < chunkCount; i++) {
		chunks.push_back(blockMap.getChunk(chunkStart + i));
	}
}