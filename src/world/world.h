#pragma once
#include "block/block_map.h"
#include "block/block_types.h"
#include "world/content/content.h"

namespace bf {
	class World {
    public:
        BlockMap map;
        BlockTypes blocks;

        Content content;

        World();
    };
}