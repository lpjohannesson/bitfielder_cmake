#pragma once
#include "world/entity/systems/entity_system.h"
#include "../components/sprite_animator_component.h"
#include "../components/sprite_component.h"

namespace bf {
	class SpriteAnimatorSystem : public EntitySystem {
    public:
        static void playAnimation(SpriteAnimatorComponent &animator, SpriteAnimation &animation);

        void update(World &world) override;
	};
}