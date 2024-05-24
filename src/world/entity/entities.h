#pragma once
#include <entt/entt.hpp>
#include "systems/entity_system.h"

namespace bf {
	class Entities {
    public:
        entt::registry registry;
        std::vector<EntitySystem*> systems;

        void addSystem(EntitySystem &system);

        void update(World &world);
    };
}