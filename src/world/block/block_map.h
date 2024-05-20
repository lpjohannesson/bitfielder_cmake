#pragma once
#include <entt/entt.hpp>
#include "block_chunk.h"

namespace bf {
	class BlockMap {
	private:
		std::unordered_map<int, entt::entity> chunkMap;

	public:
		entt::registry registry;

        entt::entity *getChunk(int index) const;
        entt::entity createChunk(int index);
	};
}