#pragma once
#include <vector>
#include "block_map.h"
#include "block_chunk.h"

namespace bf {
	template<typename T>
	class BlockSample {
	private:
		int chunkStart;
		std::vector<T*> chunks;

	public:
		inline int getChunkStart() const { return chunkStart; }
		inline int getChunkCount() const { return chunks.size(); }

		inline BlockChunk* getChunk(int chunkIndex) const {
			return chunks[chunkIndex - chunkStart];
		}

		inline int getBlockIndex(glm::ivec2 position) const {
			int chunkIndex = BlockChunk::getChunkIndex(position.x);
			BlockChunk* chunk = getChunk(chunkIndex);

			if (chunk == nullptr) {
				return 0;
			}

			glm::ivec2 chunkPosition = BlockChunk::worldToChunk(position, chunkIndex);
			return chunk->getBlockIndex(chunkPosition);
		}

        inline int sampleBlockIndex(glm::ivec2 position) const {
			int chunkIndex = BlockChunk::getChunkIndex(position.x);
			BlockChunk* chunk = getChunk(chunkIndex);

			if (chunk == nullptr) {
				return 0;
			}

			glm::ivec2 chunkPosition = BlockChunk::worldToChunk(position, chunkIndex);
			return chunk->sampleBlockIndex(chunkPosition);
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