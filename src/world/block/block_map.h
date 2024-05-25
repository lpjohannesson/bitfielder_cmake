#pragma once
#include <unordered_map>
#include "block_chunk.h"

namespace bf {
	class BlockMap {
	public:
		std::unordered_map<int, BlockChunk> chunks;

		static int getChunkIndex(int blockX);

		static glm::ivec2 worldToChunk(glm::ivec2 position, int chunkIndex);

        BlockChunk *getChunk(int index) const;
        BlockChunk &createChunk(int index);
	};
}