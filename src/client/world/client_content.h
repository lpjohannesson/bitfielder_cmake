#pragma once
#include <glm/glm.hpp>
#include "entity/systems/client_player_system.h"
#include "block/renderers/basic_block_renderer.h"

namespace bf {
    class WorldScene;
    
    class ClientContent {
    public:
        ClientPlayerSystem clientPlayerSystem;

        BasicBlockRenderer testBlockRenderer;

        entt::entity spawnRemotePlayer(WorldScene &scene, glm::vec2 position);
        entt::entity spawnLocalPlayer(WorldScene &scene, glm::vec2 position);

        void loadContent(WorldScene &scene);
    };
}