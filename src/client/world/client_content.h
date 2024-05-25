#pragma once
#include "world/world.h"
#include "client/world/world_renderer.h"
#include "client/world/block/renderers/basic_block_renderer.h"
#include "client/world/entity/systems/client_player_system.h"

namespace bf {
    class ClientContent {
    public:
        ClientPlayerSystem clientPlayerSystem;

        BasicBlockRenderer testBlockRenderer;

        void loadContent(World &world);
    };
}