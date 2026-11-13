#pragma once
#include <glm/glm.hpp>
#include "gfx/sprite/sprite_frames.h"

namespace bf {
    struct BlockParticleComponent {
        glm::vec2 size{};
        SpriteFrames frames;
    };
}