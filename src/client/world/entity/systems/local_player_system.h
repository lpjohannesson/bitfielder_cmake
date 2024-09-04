#pragma once
#include "client_entity_system.h"
#include "gfx/sprite/sprite_set.h"
#include "../components/local_player_component.h"
#include "../components/sprite_component.h"
#include "../components/sprite_set_component.h"
#include "../components/animator_component.h"
#include "../systems/animator_system.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/velocity_component.h"
#include "world/entity/components/body_component.h"
#include "world/entity/components/animation_component.h"
#include "world/entity/components/flip_component.h"
#include "world/entity/components/aim_component.h"
#include "world/entity/components/inventory_component.h"
#include "sound/sound.h"

namespace bf {
    struct LocalPlayerData {
        WorldScene &scene;

        LocalPlayerComponent &localPlayer;
        PositionComponent &position;
        VelocityComponent &velocity;
        BodyComponent &body;
        AnimationComponent &animation;
        AnimatorComponent &animator;
        FlipComponent &flip;
        AimComponent &aim;
        InventoryComponent &inventory;

        glm::vec2 movement;
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
        float speed, acceleration, gravity, maxGravity, jumpImpulse, jumpStop, maxFloorTime, maxJumpTime, maxBlockTime, maxBlockTweenTime;

        void update(WorldScene &scene) override;

        LocalPlayerSystem();
	};
}