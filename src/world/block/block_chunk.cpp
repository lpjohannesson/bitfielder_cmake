#include "block_chunk.h"

using namespace bf;

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

BlockChunk::BlockChunk(int mapIndex) {
	this->mapIndex = mapIndex;

	// Clear chunk
	for (int y = 0; y < SIZE.y; y++) {
		for (int x = 0; x < SIZE.x; x++) {
			glm::ivec2 position = { x, y };
			
			setBlockIndex(position, 0);
		}
	}
}