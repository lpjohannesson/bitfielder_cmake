#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "world/content.h"
#include "entity/systems/sprite_animator_system.h"
#include "entity/systems/local_player_system.h"
#include "block/renderers/basic_block_renderer.h"

namespace bf {
    class WorldScene;

    enum class PlayerAnimation {
        IDLE, WALK, JUMP, FALL, SLIDE
    };
    
    class ClientContent {
    public:
        SpriteAnimationSet playerAnimations;

        SpriteAnimatorSystem spriteAnimatorSystem;
        LocalPlayerSystem localPlayerSystem;

        BasicBlockRenderer testBlockRenderer;

        entt::entity player;

        void createPlayer(entt::entity player, WorldScene &scene, const PlayerSpawnProperties &spawnProperties);
        void createLocalPlayer(entt::entity player, WorldScene &scene, const PlayerSpawnProperties &spawnProperties);

        ClientContent(WorldScene &scene);
    };
}