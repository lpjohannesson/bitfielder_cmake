#pragma once
#include "systems/sprite_system.h"

namespace bf {
    class World;

    class EntityRenderer {
    public:
        SpriteSystem spriteSystem;

        void render(const World &world);

        inline EntityRenderer(const SpriteRenderer &spriteRenderer) : spriteSystem(spriteRenderer) {}
    };
}