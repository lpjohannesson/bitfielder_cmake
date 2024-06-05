#pragma once
#include "client_entity_system.h"
#include "gfx/sprite/animation/sprite_frames.h"
#include "../components/local_player_component.h"
#include "../components/sprite_component.h"
#include "../components/sprite_animator_component.h"
#include "../systems/sprite_animator_system.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/velocity_component.h"
#include "world/entity/components/body_component.h"
#include "world/entity/components/sprite_flip_component.h"

namespace bf {
    struct LocalPlayerData {
        glm::vec2 movement;
        LocalPlayerComponent *localPlayer;
        PositionComponent *position;
        VelocityComponent *velocity;
        BodyComponent *body;
        SpriteComponent *sprite;
        SpriteFlipComponent *spriteFlip;
        SpriteAnimationComponent *spriteAnimation;
        SpriteAnimatorComponent *spriteAnimator;
    };

	class LocalPlayerSystem : public ClientEntitySystem {
    private:
        void move(LocalPlayerData &playerData);
        void selectItems(LocalPlayerData &playerData);
        void animate(LocalPlayerData &playerData);
        bool tryModifyBlock(LocalPlayerData &playerData);

    public:
        float speed, acceleration, gravity, jumpImpulse, jumpStop, maxFloorTime, maxJumpTime, maxBlockTime, maxBlockTweenTime;

        void update(World &world) override;

        LocalPlayerSystem();
	};
}