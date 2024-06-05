#pragma once
#include <string>
#include <glm/glm.hpp>

namespace bf {
	class Color {
    private:
        static float parseByte(unsigned int value, int shift);

    public:
        static glm::vec4 parseHex(std::string hex);
    };
}