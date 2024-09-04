#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "gfx/core/texture.h"
#include "world/content.h"
#include "sound/sound_set.h"
#include "block/block_sounds.h"
#include "entity/sprite_aim_set.h"
#include "entity/systems/animator_system.h"
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
        Sound placeSound, destroySound, jumpSound, groundSound, selectItemSound;

        BlockSounds blockSounds;

        SpriteAimSet playerSkinSprites, playerOverlaySprites;

        AnimationSet playerAnimations;

        EffectSpriteProperties placeEffectProperties, destroyEffectProperties, groundEffectProperties;

        AnimatorSystem animatorSystem;
        ParticleSystem particleSystem;
        EffectSpriteSystem effectSpriteSystem;
        LocalPlayerSystem localPlayerSystem;

        entt::entity player;

        Texture skinTexture;

        void createPlayer(entt::entity player, WorldScene &scene);
        void createLocalPlayer(entt::entity player, WorldScene &scene);

        ClientContent(WorldScene &scene);
    };
}