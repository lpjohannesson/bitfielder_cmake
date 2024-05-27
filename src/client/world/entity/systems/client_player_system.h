#pragma once
#include "world/entity/systems/entity_system.h"

namespace bf {
	class ClientPlayerSystem : public EntitySystem {
    public:
        float speed, acceleration, gravity, jumpImpulse, jumpStop;

        void update(World &world) override;

        ClientPlayerSystem();
	};
}