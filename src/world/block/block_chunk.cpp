#include "block_chunk.h"

using namespace bf;

int BlockChunk::getIndex(glm::ivec2 position) const {
	return data[position.y][position.x];
}

void BlockChunk::setIndex(glm::ivec2 position, int index) {
	data[position.y][position.x] = index;
}

BlockChunk::BlockChunk() {
	for (int y = 0; y < SIZE.y; y++) {
		for (int x = 0; x < SIZE.x; x++) {
			glm::ivec2 position = { x, y };
			
			setIndex(position, rand() % 2);
		}
	}
}

BlockChunk::~BlockChunk() {
#ifdef BF_CLIENT
	// Delete mesh
    if (blockMesh != nullptr) {
        delete blockMesh;
    }
#endif
}