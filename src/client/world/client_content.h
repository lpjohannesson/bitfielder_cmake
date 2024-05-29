#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "entity/systems/sprite_animator_system.h"
#include "entity/systems/local_player_system.h"
#include "block/renderers/basic_block_renderer.h"

namespace bf {
    class WorldScene;
    
    class ClientContent {
    public:
        SpriteFrames playerFrames;
        SpriteAnimation playerIdle, playerWalk, playerJump, playerSlide;

        SpriteAnimatorSystem spriteAnimatorSystem;
        LocalPlayerSystem localPlayerSystem;

        BasicBlockRenderer testBlockRenderer;

        entt::entity player;

        void createPlayer(entt::entity player, WorldScene &scene, glm::vec2 position);
        void createLocalPlayer(entt::entity player, WorldScene &scene, glm::vec2 position);

        ClientContent(WorldScene &scene);
    };
}