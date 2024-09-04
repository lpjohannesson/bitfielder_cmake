#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "core/box2.h"

namespace bf {
    class SpriteSet {
    public:
        std::vector<Box2> boxes;

        void loadSprites(Box2 uvBox, glm::ivec2 counts);
    };
}