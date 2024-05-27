#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "entity/systems/client_player_system.h"
#include "block/renderers/basic_block_renderer.h"

namespace bf {
    class WorldScene;
    
    class ClientContent {
    public:
        ClientPlayerSystem clientPlayerSystem;

        BasicBlockRenderer testBlockRenderer;

        entt::entity player;

        void createPlayer(entt::entity player, WorldScene &scene, glm::vec2 position);
        void createLocalPlayer(entt::entity player, WorldScene &scene, glm::vec2 position);

        void loadContent(WorldScene &scene);
    };
}