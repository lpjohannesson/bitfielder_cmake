#include "block_chunk.h"
#include "block_sample.h"
#include "block_map.h"

using namespace bf;

#define GET_BLOCK(MAP)\
int chunkIndex = getChunkIndex(position.x);\
BlockChunk *chunk = MAP.getChunk(chunkIndex);\
\
if (chunk == nullptr) {\
	return nullptr;\
}\
\
glm::ivec2 chunkPosition = worldToChunk(position, chunkIndex);\
return chunk->getBlockChecked(chunkPosition);

int BlockChunk::getChunkIndex(int blockX) {
	return (int)glm::floor(blockX / (float)SIZE.x);
}

glm::ivec2 BlockChunk::worldToChunk(glm::ivec2 position, int chunkIndex) {
	return { position.x - chunkIndex * SIZE.x, position.y };  
}

BlockData *BlockChunk::getWorldBlock(const BlockMap<BlockChunk> &map, glm::ivec2 position) {
	GET_BLOCK(map);
}

BlockData *BlockChunk::getSampleBlock(const BlockSample<BlockChunk> &sample, glm::ivec2 position) {
	GET_BLOCK(sample);
}

BlockData *BlockChunk::getBlock(glm::ivec2 position) const {
	return (BlockData*)&data[position.y][position.x];
}

BlockData *BlockChunk::getBlockChecked(glm::ivec2 position) const {
	// Check vertical
	if (position.y < 0 || position.y >= SIZE.y) {
		return nullptr;
	}

	return getBlock(position);
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