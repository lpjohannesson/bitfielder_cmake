#pragma once
#include "world/entity/systems/entity_system.h"
#include "../components/sprite_animator_component.h"
#include "world/entity/components/sprite_animation_component.h"

namespace bf {
	class SpriteAnimatorSystem : public EntitySystem {
    public:
        static bool playAnimation(SpriteAnimatorComponent &animator, SpriteAnimationComponent &animation, int animationIndex);

        void update(World &world) override;
	};
}