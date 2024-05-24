#pragma once
#include "world/entity/systems/entity_system.h"

namespace bf {
	class ClientPlayerSystem : public EntitySystem {
    public:
        void update(World &world) override;
	};
}