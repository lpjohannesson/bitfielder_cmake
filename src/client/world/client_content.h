#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "world/content.h"
#include "sound/sound_set.h"
#include "block/block_sounds.h"
#include "entity/systems/sprite_animator_system.h"
#include "entity/systems/local_player_system.h"
#include "entity/systems/particle_system.h"
#include "entity/systems/effect_sprite_system.h"

namespace bf {
    class WorldScene;

    enum class PlayerAnimation {
        IDLE, WALK, JUMP, FALL, SLIDE, PUNCH
    };
    
    class ClientContent {
    public:
        SoundSet soundSet;
        ALuint placeSound, destroySound, jumpSound, groundSound;

        BlockSounds blockSounds;

        SpriteFrames playerForwardFrames, playerUpFrames, playerDownFrames;
        SpriteAnimationSet playerAnimations;

        EffectSpriteProperties placeEffectProperties, destroyEffectProperties, groundEffectProperties;

        SpriteAnimatorSystem spriteAnimatorSystem;
        LocalPlayerSystem localPlayerSystem;
        ParticleSystem particleSystem;
        EffectSpriteSystem effectSpriteSystem;

        entt::entity player;

        void createPlayer(entt::entity player, WorldScene &scene);
        void createLocalPlayer(entt::entity player, WorldScene &scene);

        void end(WorldScene &scene);

        ClientContent(WorldScene &scene);
    };
}