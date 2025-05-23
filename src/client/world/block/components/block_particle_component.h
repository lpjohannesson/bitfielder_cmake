#pragma once
#include <glm/glm.hpp>
#include "gfx/sprite/sprite_set.h"

namespace bf {
    struct BlockParticleComponent {
        glm::vec2 size{};
        SpriteSet sprites;
    };
}