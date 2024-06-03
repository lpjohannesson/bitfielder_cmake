#pragma once
#include <vector>
#include "block_map.h"
#include "block_chunk.h"

namespace bf {
	template<typename T>
	class BlockSample {
	private:
		int chunkStart;

	public:
		std::vector<T*> chunks;

		inline T *getChunk(int chunkIndex) const {
			return chunks[chunkIndex - chunkStart];
		}

        inline BlockSample(const BlockMap<T>& blockMap, int blockStartX, int blockEndX) {
			// Get chunk extents
			chunkStart = BlockChunk::getChunkIndex(blockStartX);
			int chunkEnd = BlockChunk::getChunkIndex(blockEndX);
			int chunkCount = chunkEnd - chunkStart + 1;

			// Load chunks
			chunks.reserve(chunkCount);

			for (int i = 0; i < chunkCount; i++) {
				chunks.push_back(blockMap.getChunk(chunkStart + i));
			}
		}
	};
}