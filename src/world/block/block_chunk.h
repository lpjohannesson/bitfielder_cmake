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
		unsigned int data[SIZE.y][SIZE.x];

	public:
#ifdef BF_CLIENT
		BlockMesh *blockMesh = nullptr;
#endif

		int getIndex(glm::ivec2 position) const;
		void setIndex(glm::ivec2 position, int index);

		BlockChunk();
		~BlockChunk();
	};
}