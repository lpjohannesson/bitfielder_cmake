#pragma once
#include "entity_system.h"

namespace bf {
	class BodySystem : public EntitySystem {
    public:
        void update(World &world) override;
	};
}