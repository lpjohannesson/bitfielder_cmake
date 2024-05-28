#pragma once
#include "world/entity/systems/entity_system.h"

namespace bf {
	class LocalPlayerSystem : public EntitySystem {
    public:
        float speed, acceleration, gravity, jumpImpulse, jumpStop, maxFloorTime;

        void update(World &world) override;

        LocalPlayerSystem();
	};
}