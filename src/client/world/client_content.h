#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "world/content.h"
#include "entity/systems/sprite_animator_system.h"
#include "entity/systems/local_player_system.h"
#include "entity/systems/particle_system.h"

namespace bf {
    class WorldScene;

    enum class PlayerAnimation {
        IDLE, WALK, JUMP, FALL, SLIDE, PUNCH
    };
    
    class ClientContent {
    public:
        SpriteFrames playerForwardFrames, playerUpFrames, playerDownFrames;
        SpriteAnimationSet playerAnimations;

        SpriteAnimatorSystem spriteAnimatorSystem;
        LocalPlayerSystem localPlayerSystem;
        ParticleSystem particleSystem;

        entt::entity player;

        void createPlayer(entt::entity player, WorldScene &scene);
        void createLocalPlayer(entt::entity player, WorldScene &scene);

        ClientContent(WorldScene &scene);
    };
}