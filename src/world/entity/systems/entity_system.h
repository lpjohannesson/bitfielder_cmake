#pragma once
#include <entt/entt.hpp>

namespace bf {
    class World;
    
	class EntitySystem {
    public:
        inline virtual void update(World &world) {}
    };
}