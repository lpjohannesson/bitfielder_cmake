#pragma once
#include <entt/entt.hpp>

namespace bf {
    class World;
    
	class RenderSystem {
    public:
        inline virtual void render(const World &world) {}
    };
}