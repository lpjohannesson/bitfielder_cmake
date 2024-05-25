#pragma once
#include <vector>
#include "block_map.h"

namespace bf {
	class BlockSample {
	private:
		int chunkStart;
		std::vector<BlockChunk*> chunks;

	public:
		inline int getChunkStart() const { return chunkStart; }
		inline int getChunkCount() const { return chunks.size(); }

		BlockChunk* getChunk(int chunkIndex) const;

		int getBlockIndex(glm::ivec2 position) const;
        int sampleBlockIndex(glm::ivec2 position) const;

        BlockSample(const BlockMap& blockMap, int startX, int endX);
	};
}