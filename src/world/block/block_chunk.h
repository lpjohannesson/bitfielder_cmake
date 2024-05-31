#pragma once
#include <glm/glm.hpp>
#include "block_map_element.h"

namespace bf {
	struct BlockData {
		unsigned int frontIndex, backIndex;
	};

	class BlockChunk : public BlockMapElement {
	public:
		static constexpr glm::ivec2 SIZE{ 16, 16 };

		BlockData data[SIZE.y][SIZE.x];

		static int getChunkIndex(int blockX);
		static glm::ivec2 worldToChunk(glm::ivec2 position, int chunkIndex);

		BlockData *getBlock(glm::ivec2 position) const;
		BlockData *sampleBlock(glm::ivec2 position) const;

		BlockChunk(int mapIndex);
	};
}