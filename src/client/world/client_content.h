#pragma once
#include "world/world.h"
#include "world_renderer.h"
#include "entity/systems/client_player_system.h"
#include "block/renderers/basic_block_renderer.h"

namespace bf {
    class ClientContent {
    public:
        ClientPlayerSystem clientPlayerSystem;

        BasicBlockRenderer testBlockRenderer;

        void loadContent(World &world);
    };
}