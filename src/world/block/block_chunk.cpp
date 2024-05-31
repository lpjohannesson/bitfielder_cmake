#include "block_chunk.h"

using namespace bf;

int BlockChunk::getChunkIndex(int blockX) {
	return glm::floor(blockX / (float)BlockChunk::SIZE.x);
}

glm::ivec2 BlockChunk::worldToChunk(glm::ivec2 position, int chunkIndex) {
	return { position.x - chunkIndex * BlockChunk::SIZE.x, position.y };  
}

BlockData *BlockChunk::getBlock(glm::ivec2 position) const {
	return (BlockData*)&data[position.y][position.x];
}

BlockData *BlockChunk::sampleBlock(glm::ivec2 position) const {
	// Check vertical
	if (position.y < 0 || position.y >= SIZE.y) {
		return nullptr;
	}

	return (BlockData*)getBlock(position);
}

BlockChunk::BlockChunk(int mapIndex) : BlockMapElement(mapIndex) {
	// Clear chunk
	for (int y = 0; y < SIZE.y; y++) {
		for (int x = 0; x < SIZE.x; x++) {
			BlockData *blockData = getBlock({ x, y });
			
			blockData->frontIndex = 0;
			blockData->backIndex = 0;
		}
	}
}