#pragma once
#include <glm/glm.hpp>

namespace bf {
	class Direction {
    public:
        static void targetAxis(float &axis, float target, float change);
    };
}