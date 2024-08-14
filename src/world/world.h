#pragma once
#include "registry/entity_registry.h"
#include "block/block_map.h"
#include "block/block_chunk.h"
#include "entity/entities.h"
#include "world/content.h"

namespace bf {
	class World {
    private:
        bool isBlockAttachable(int index);

    public:
        EntityRegistry blocks, items;
        BlockMap<BlockChunk> map;

        Entities entities;

        Content content;
        
        bool isBlockPlaceable(glm::ivec2 position, bool onFrontLayer);

        void updateBlock(glm::ivec2 position, Box2i &resultBox);

        void update();

        inline World() : content(*this) {}
    };
}