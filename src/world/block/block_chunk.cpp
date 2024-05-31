#include "block_chunk.h"

using namespace bf;

int BlockChunk::getChunkIndex(int blockX) {
	return glm::floor(blockX / (float)BlockChunk::SIZE.x);
}

glm::ivec2 BlockChunk::worldToChunk(glm::ivec2 position, int chunkIndex) {
	return { position.x - chunkIndex * BlockChunk::SIZE.x, position.y };  
}

int BlockChunk::getBlockIndex(glm::ivec2 position) const {
	return data[position.y][position.x];
}

int BlockChunk::sampleBlockIndex(glm::ivec2 position) const {
	// Check vertical
	if (position.y < 0 || position.y >= SIZE.y) {
		return 0;
	}

	return getBlockIndex(position);
}

void BlockChunk::setBlockIndex(glm::ivec2 position, int index) {
	data[position.y][position.x] = index;
}

BlockChunk::BlockChunk(int mapIndex) : BlockMapElement(mapIndex) {
	// Clear chunk
	for (int y = 0; y < SIZE.y; y++) {
		for (int x = 0; x < SIZE.x; x++) {
			glm::ivec2 position = { x, y };
			
			setBlockIndex(position, 0);
		}
	}
}