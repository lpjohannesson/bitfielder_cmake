#pragma once
#include "block/block_map.h"
#include "block/block_types.h"
#include "entity/entities.h"
#include "world/content.h"

#ifdef BF_CLIENT
#include "client/world/world_renderer.h"
#endif

namespace bf {
	class World {
    public:
        BlockMap map;
        BlockTypes blocks;

        Entities entities;

        Content content;

#ifdef BF_CLIENT
        WorldRenderer renderer;
#endif

        void update();

        World();
    };
}