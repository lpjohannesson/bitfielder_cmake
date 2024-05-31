#pragma once
#include <glm/glm.hpp>
#include "block_map_element.h"

namespace bf {
	class BlockChunk : public BlockMapElement {
	public:
		static constexpr glm::ivec2 SIZE{ 16, 16 };

	public:
		unsigned int data[SIZE.y][SIZE.x];

		static int getChunkIndex(int blockX);
		static glm::ivec2 worldToChunk(glm::ivec2 position, int chunkIndex);

		int getBlockIndex(glm::ivec2 position) const;
		int sampleBlockIndex(glm::ivec2 position) const;

		void setBlockIndex(glm::ivec2 position, int index);

		BlockChunk(int mapIndex);
	};
}