#pragma once
#include <string>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace bf {
	class Color {
    private:
        static float parseByte(unsigned int value, int shift);

    public:
        static glm::vec4 parseHex(std::string hex);
        static glm::vec4 getSurfacePixel(SDL_Surface *surface, glm::ivec2 position);
    };
}