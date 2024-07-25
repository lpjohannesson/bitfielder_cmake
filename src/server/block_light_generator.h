#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace bf {
    class BlockLightCell {
        glm::vec2 position;
    };

    class BlockLightGenerator {
    public:
        std::vector<BlockLightCell> cells;
    };
}