#pragma once
#include "block/block_map.h"
#include "block/block_types.h"
#include "entity/entities.h"
#include "world/content.h"

namespace bf {
	class World {
    public:
        BlockMap map;
        BlockTypes blocks;

        Entities entities;

        Content content;

        void update();

        World();
    };
}