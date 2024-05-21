#pragma once
#include <unordered_map>
#include "block_chunk.h"

namespace bf {
	class BlockMap {
	public:
		std::unordered_map<int, BlockChunk> chunks;

        BlockChunk *getChunk(int index) const;
        BlockChunk &createChunk(int index);
	};
}