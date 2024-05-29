#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "core/box2.h"

namespace bf {
    class SpriteFrames {
    public:
        std::vector<Box2> frames;

        void loadFrames(Box2 uvBox, glm::ivec2 frameCounts);
    };
}