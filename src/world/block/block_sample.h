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

		inline void sampleChunks(const BlockMap<T>& map, int start, int end) {
			chunkStart = start;
			int chunkCount = end - start + 1;

			chunks.reserve(chunkCount);

			for (int i = 0; i < chunkCount; i++) {
				chunks.push_back(map.getChunk(chunkStart + i));
			}
		}

        inline void sampleBlocks(const BlockMap<T>& map, int startX, int endX) {
			// Get chunk extents
			chunkStart = BlockChunk::getChunkIndex(startX);
			int chunkEnd = BlockChunk::getChunkIndex(endX);
			
			sampleChunks(map, chunkStart, chunkEnd);
		}
	};
}