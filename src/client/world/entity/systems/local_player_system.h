#pragma once
#include "world/entity/systems/entity_system.h"
#include "gfx/sprite/animation/sprite_frames.h"
#include "../components/local_player_component.h"
#include "../components/sprite_component.h"
#include "../components/sprite_animator_component.h"
#include "../systems/sprite_animator_system.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/body_component.h"
#include "world/entity/components/sprite_flip_component.h"

namespace bf {
    struct LocalPlayerData {
        glm::vec2 movement;
        LocalPlayerComponent *localPlayer;
        PositionComponent *position;
        BodyComponent *body;
        SpriteComponent *sprite;
        SpriteFlipComponent *spriteFlip;
        SpriteAnimationComponent *spriteAnimation;
        SpriteAnimatorComponent *spriteAnimator;
    };

    class WorldScene;

	class LocalPlayerSystem : public EntitySystem {
    private:
        void move(LocalPlayerData &playerData);
        void animate(LocalPlayerData &playerData);
        bool tryModifyBlock(LocalPlayerData &playerData);

    public:
        float speed, acceleration, gravity, jumpImpulse, jumpStop, maxFloorTime, maxJumpTime, maxBlockTime;
        WorldScene *scene;

        void loadContent(WorldScene &scene);

        void update(World &world) override;

        LocalPlayerSystem();
	};
}