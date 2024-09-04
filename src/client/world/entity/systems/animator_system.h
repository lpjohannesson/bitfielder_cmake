#pragma once
#include "client_entity_system.h"
#include "../components/animator_component.h"
#include "world/entity/components/animation_component.h"

namespace bf {
	class AnimatorSystem : public ClientEntitySystem {
    public:
        static bool playAnimation(AnimatorComponent &animator, AnimationComponent &animation, int index);

        void update(WorldScene &scene) override;
	};
}