#pragma once
#include "block/block_map.h"
#include "block/block_chunk.h"
#include "block/block_types.h"
#include "entity/entities.h"
#include "world/content.h"

namespace bf {
	class World {
    private:
        bool isBlockAttachable(int index);

    public:
        BlockMap<BlockChunk> map;
        BlockTypes blocks;

        Entities entities;

        Content content;
        
        bool isBlockPlaceable(glm::ivec2 position, bool onFrontLayer);

        void update();

        inline World() : content(*this) {}
    };
}