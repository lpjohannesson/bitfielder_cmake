#pragma once
#include <entt/entt.hpp>

namespace bf {
    class World;

    class BlockContent {
    private:
        void createSolid(World &world, entt::entity block);

    public:
        entt::entity
            air,
            dirt,
            grass,
            stone,
            woodLog,
            woodPlanks,
            stick,
            leaves,
            bush,
            mushroom,
            wheat,
            iron,
            gold,
            wool;
        
        BlockContent(World &world);
    };
}