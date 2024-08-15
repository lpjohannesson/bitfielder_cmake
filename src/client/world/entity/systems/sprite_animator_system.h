#pragma once
#include "client_entity_system.h"
#include "../components/sprite_animator_component.h"
#include "world/entity/components/sprite_animation_component.h"

namespace bf {
	class SpriteAnimatorSystem : public ClientEntitySystem {
    public:
        static bool playAnimation(SpriteAnimatorComponent &spriteAnimator, SpriteAnimationComponent &spriteAnimation, int animationIndex);

        void update(WorldScene &scene) override;
	};
}