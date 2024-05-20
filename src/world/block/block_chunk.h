#pragma once
#include <glm/glm.hpp>

namespace bf {
	class BlockChunk {
	public:
		static constexpr glm::ivec2 SIZE{ 16, 16 };

	private:
		unsigned int data[SIZE.y][SIZE.x];

	public:
		int getIndex(glm::ivec2 position) const;
		void setIndex(glm::ivec2 position, int index);

		BlockChunk();
	};
}