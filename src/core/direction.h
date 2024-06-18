#pragma once
#include <glm/glm.hpp>

namespace bf {
	class Direction {
    public:
        static int posmod(int a, int b);
        static void targetAxis(float &axis, float target, float change);
    };
}