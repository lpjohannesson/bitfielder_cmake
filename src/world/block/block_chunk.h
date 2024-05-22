#pragma once
#include <glm/glm.hpp>

#ifdef BF_CLIENT
#include "client/world/block/block_mesh.h"
#endif

namespace bf {
	class BlockChunk {
	public:
		static constexpr glm::ivec2 SIZE{ 16, 16 };

	private:
		int mapIndex;
		unsigned int data[SIZE.y][SIZE.x];

	public:
#ifdef BF_CLIENT
		BlockMesh *blockMesh = nullptr;
#endif
		inline int getMapIndex() const { return mapIndex; }

		int getBlockIndex(glm::ivec2 position) const;
		void setBlockIndex(glm::ivec2 position, int index);

		BlockChunk(int mapIndex);
		~BlockChunk();
	};
}