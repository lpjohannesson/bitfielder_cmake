#pragma once
#include <glm/glm.hpp>
#include "block_map_element.h"

namespace bf {
	struct BlockData {
		unsigned int frontIndex, backIndex;
	};

	template <typename T>
	class BlockSample;

	template <typename T>
	class BlockMap;

	class BlockChunk : public BlockMapElement {
	public:
		static constexpr glm::ivec2 SIZE{ 16, 256 };

		BlockData data[SIZE.y][SIZE.x];

		static int getChunkIndex(int blockX);
		static glm::ivec2 worldToChunk(glm::ivec2 position, int chunkIndex);

		static BlockData *getWorldBlock(const BlockMap<BlockChunk> &map, glm::ivec2 position);
		static BlockData *getSampleBlock(const BlockSample<BlockChunk> &sample, glm::ivec2 position);

		BlockData *getBlock(glm::ivec2 position) const;
		BlockData *getBlockChecked(glm::ivec2 position) const;

		BlockChunk(int mapIndex);
	};
}