#pragma once
#include "client_entity_system.h"
#include "gfx/sprite/sprite_frames.h"
#include "../components/local_player_component.h"
#include "../components/sprite_component.h"
#include "../components/sprite_animator_component.h"
#include "../components/sprite_aim_component.h"
#include "../systems/sprite_animator_system.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/velocity_component.h"
#include "world/entity/components/body_component.h"
#include "world/entity/components/sprite_flip_component.h"
#include "world/entity/components/aim_component.h"

namespace bf {
    struct LocalPlayerData {
        WorldScene &scene;

        glm::vec2 movement;
        LocalPlayerComponent &localPlayer;
        PositionComponent &position;
        VelocityComponent &velocity;
        BodyComponent &body;
        SpriteComponent &sprite;
        SpriteFlipComponent &spriteFlip;
        SpriteAnimationComponent &spriteAnimation;
        SpriteAnimatorComponent &spriteAnimator;
        AimComponent &aim;
        SpriteAimComponent &spriteAim;

        bool stateChanged;
    };

	class LocalPlayerSystem : public ClientEntitySystem {
    private:
        void move(LocalPlayerData &data);
        void jump(LocalPlayerData &data);
        void selectItems(LocalPlayerData &data);
        void applyAim(LocalPlayerData &data);
        void animate(LocalPlayerData &data);
        bool tryModifyBlock(LocalPlayerData &data);

    public:
        float speed, acceleration, gravity, jumpImpulse, jumpStop, maxFloorTime, maxJumpTime, maxBlockTime, maxBlockTweenTime;

        void update(WorldScene &scene) override;

        LocalPlayerSystem();
	};
}