#include "color.h"
#include <sstream>

using namespace bf;

float Color::parseByte(unsigned int value, int shift) {
    unsigned char shiftedValue = (unsigned char)(value >> shift * 8);

    return ((float)shiftedValue) / 255.0f;
}

glm::vec4 Color::parseHex(std::string hex) {
    unsigned int value;

    std::stringstream ss;
    ss << std::hex << hex;
    ss >> value;

    return {
        parseByte(value, 2),
        parseByte(value, 1),
        parseByte(value, 0),
        1.0f
    };
}

glm::vec4 Color::getSurfacePixel(SDL_Surface *surface, glm::ivec2 position) {
    Uint8 *pixelStart =
        (Uint8*)surface->pixels + position.y * surface->pitch + position.x * surface->format->BytesPerPixel;
    
    return glm::vec4(*(pixelStart + 0), *(pixelStart + 1), *(pixelStart + 2), *(pixelStart + 3)) / 255.0f;
}