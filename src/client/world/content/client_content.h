#pragma once
#include "world/world.h"
#include "client/world/world_renderer.h"
#include "client/world/block/renderers/basic_block_renderer.h"

namespace bf {
    class ClientContent {
    public:
        BasicBlockRenderer testBlockRenderer;

        void loadContent(World &world, WorldRenderer &renderer);
    };
}