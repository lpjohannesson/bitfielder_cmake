#pragma once
#include "entity/systems/client_player_system.h"
#include "block/renderers/basic_block_renderer.h"

namespace bf {
    class WorldScene;
    
    class ClientContent {
    public:
        ClientPlayerSystem clientPlayerSystem;

        BasicBlockRenderer testBlockRenderer;

        void loadContent(WorldScene &scene);
    };
}