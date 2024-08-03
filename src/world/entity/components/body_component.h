#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "world/block/block_chunk.h"

namespace bf {
	struct BlockCollision {
		BlockData *blockData;
		glm::ivec2 blockPosition;
		glm::vec2 normal;
	};

	struct BodyComponent {
		glm::vec2 size{};

		bool isOnFloor = false, isOnCeiling = false;

		std::vector<BlockCollision> blockCollisions;
	};
}